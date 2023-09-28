//
// Created by Jurgen Zellhuber.
//

#include <array>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>

#include "Animation.hpp"
#include "Global.hpp"
#include "Enemy.hpp"
#include "EnemyManager.hpp"

EnemyManager::EnemyManager() :
        shoot_distribution(0, ENEMY_SHOOT_CHANCE)
{

    reset(0);

    enemy_bullet_texture.loadFromFile("../Resources/Images/EnemyBullet.png");

    enemy_bullet_sprite.setTexture(enemy_bullet_texture);

    for (unsigned char a = 0; a < ENEMY_TYPES; a++)
    {
        enemy_animations.push_back(Animation(1 + move_pause, BASE_SIZE, "../Resources/Images/Enemy" + std::to_string(static_cast<unsigned short>(a)) + ".png"));
    }
}

bool EnemyManager::reached_player(unsigned short i_player_y) const
{
    for (const Enemy& enemy : enemies)
    {
        if (enemy.get_y() > i_player_y - 0.5f * BASE_SIZE)
        {
            //Cuando un enemigo llega a la altura del jugador, se acaba el juego.
            return 1;
        }
    }

    return 0;
}

void EnemyManager::draw(sf::RenderWindow& i_window)
{
    for (const Bullet& bullet : enemy_bullets)
    {
        //Dibujar la cola de la bala.
        for (unsigned char a = 0; a < bullet.previous_x.size(); a++)
        {
            enemy_bullet_sprite.setPosition(bullet.previous_x[a], bullet.previous_y[a]);
            enemy_bullet_sprite.setTextureRect(sf::IntRect(BASE_SIZE * a, 0, BASE_SIZE, BASE_SIZE));

            i_window.draw(enemy_bullet_sprite);
        }

        //Dibujar la cabeza de la bala.
        enemy_bullet_sprite.setPosition(bullet.x, bullet.y);
        enemy_bullet_sprite.setTextureRect(sf::IntRect(BASE_SIZE * bullet.previous_x.size(), 0, BASE_SIZE, BASE_SIZE));

        i_window.draw(enemy_bullet_sprite);
    }

    for (Enemy& enemy : enemies)
    {
        //Cuando un enemigo es golpeado, se pone blanco.
        sf::Color enemy_color = sf::Color(255, 255, 255);

        if (0 == enemy.get_hit_timer())
        {
            //Si no, se pinta
            switch (enemy.get_type())
            {
                case 0:
                {
                    enemy_color = sf::Color(0, 255, 255);

                    break;
                }
                case 1:
                {
                    enemy_color = sf::Color(255, 0, 255);

                    break;
                }
                case 2:
                {
                    enemy_color = sf::Color(0, 255, 0);
                }
            }
        }

        enemy_animations[enemy.get_type()].draw(enemy.get_x(), enemy.get_y(), i_window, enemy_color);
    }
}

void EnemyManager::reset(unsigned short i_level)
{
    //RESET!!!!
    unsigned char enemy_x = 0;
    unsigned char enemy_y = 0;

    std::string level_sketch = "";

    //Rand number generator
    std::random_device rd;
    std::default_random_engine gen(rd());

    //Distribuccion uniforme
    std::uniform_int_distribution<int> dist(0,2);

    //Generar el nivel al azar
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (j == 0 || j == 15)
            {
                level_sketch += ' ';
            }
            else
            {
                // Generate a random digit from 0 to 2 and append it to the level_sketch string
                level_sketch += std::to_string(dist(gen));
            }
        }

        if (i != 3)
        {
            level_sketch += '\n';
        }
    }

    move_pause = std::max<short>(ENEMY_MOVE_PAUSE_START_MIN, ENEMY_MOVE_PAUSE_START - ENEMY_MOVE_PAUSE_DECREASE * i_level);
    move_timer = move_pause;

    shoot_distribution = std::uniform_int_distribution<unsigned short>(0, std::max<short>(ENEMY_SHOOT_CHANCE_MIN, ENEMY_SHOOT_CHANCE - ENEMY_SHOOT_CHANCE_INCREASE * i_level));

    for (Animation& enemy_animation : enemy_animations)
    {
        enemy_animation.reset();
    }

    enemy_bullets.clear();

    enemies.clear();

    //8 niveles
    if (TOTAL_LEVELS <= i_level)
    {
        i_level = 0.5f * TOTAL_LEVELS + i_level % static_cast<unsigned char>(0.5f * TOTAL_LEVELS);
    }

    // Diseño
    switch (i_level)
    {
        case 0:
        {
            //level_sketch = "2011022011022011\n0011001100110011\n1100110011001100\n0 0 0 0 0 0 0 0 ";
            level_sketch = level_sketch;

            break;
        }
        case 1:
        {
            level_sketch = level_sketch;

            break;
        }
        case 2:
        {

            level_sketch = level_sketch;

            break;
        }
        case 3:
        {
            level_sketch = level_sketch;

            break;
        }
        case 4:
        {

            level_sketch = level_sketch;

            break;
        }
        case 5:
        {

            level_sketch = level_sketch;

            break;
        }
        case 6:
        {

            level_sketch = level_sketch;

            break;
        }
        case 7:
        {
            level_sketch = level_sketch;
        }
        case 8:
        {
            level_sketch = level_sketch;
        }
    }


    for (char sketch_character : level_sketch)
    {
        enemy_x++;

        switch (sketch_character)
        {
            case '\n':
            {
                enemy_x = 0;
                enemy_y++;

                break;
            }
            case '0':
            {
                enemies.push_back(Enemy(0, BASE_SIZE * (1 + enemy_x), BASE_SIZE * (2 + enemy_y)));

                break;
            }
            case '1':
            {
                enemies.push_back(Enemy(1, BASE_SIZE * (1 + enemy_x), BASE_SIZE * (2 + enemy_y)));

                break;
            }
            case '2':
            {
                enemies.push_back(Enemy(2, BASE_SIZE * (1 + enemy_x), BASE_SIZE * (2 + enemy_y)));
            }
        }
    }
}

void EnemyManager::update(std::mt19937_64& i_random_engine)
{
    std::vector<Enemy>::iterator dead_enemies_start;

    if (0 == move_timer)
    {
        move_timer = move_pause;

        for (Enemy& enemy : enemies)
        {
            enemy.move();
        }

        for (Animation& enemy_animation : enemy_animations)
        {
            //Los enemigos cambian de frame cada vez que se mueven.
            enemy_animation.change_current_frame();
        }
    }
    else
    {
        move_timer--;
    }

    for (Enemy& enemy : enemies)
    {
        enemy.update();

        if (0 == shoot_distribution(i_random_engine))
        {
            enemy.shoot(enemy_bullets);
        }
    }

    dead_enemies_start = remove_if(enemies.begin(), enemies.end(), [](const Enemy& i_enemy)
    {
        return 0 == i_enemy.get_health();
    });

    //Mas enemigos muetros = mas rapido se mueven los que quedan.
    move_pause = std::max<int>(ENEMY_MOVE_PAUSE_MIN, move_pause - ENEMY_MOVE_PAUSE_DECREASE * (enemies.end() - dead_enemies_start));

    enemies.erase(dead_enemies_start, enemies.end());

    for (Bullet& enemy_bullet : enemy_bullets)
    {
        enemy_bullet.update();
    }

    enemy_bullets.erase(remove_if(enemy_bullets.begin(), enemy_bullets.end(), [](const Bullet& i_bullet)
    {
        return 1 == i_bullet.dead;
    }), enemy_bullets.end());
}

std::vector<Bullet>& EnemyManager::get_enemy_bullets()
{
    return enemy_bullets;
}

std::vector<Enemy>& EnemyManager::get_enemies()
{
    return enemies;
}