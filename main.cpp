#include <array>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#include "Animation.hpp"
#include "DrawText.hpp"
#include "Global.hpp"
#include "Enemy.hpp"
#include "EnemyManager.hpp"
#include "Ufo.hpp"
#include "Player.hpp"

int main()
{


    bool game_over = 0;
    bool next_level = 0;

    //Nivel Actual
    unsigned short level = 0;
    unsigned short next_level_timer = NEXT_LEVEL_TRANSITION;

    //FPS independientes
    std::chrono::microseconds lag(0);

    std::chrono::steady_clock::time_point previous_time;


    //Random seed para el random engine.
    std::mt19937_64 random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    sf::Event event;

    // Crear pantalla de inicio
    sf::RenderWindow start_window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Space Invaders", sf::Style::Close);
    sf::Image icon_image;
    icon_image.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Images/icon.png");

    start_window.setIcon(icon_image.getSize().x, icon_image.getSize().y, icon_image.getPixelsPtr());

    start_window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));

    //Cargar texturas background
    sf::Texture start_texture;
    start_texture.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Images/StartScreen.png");

    sf::Sprite start_sprite;
    start_sprite.setTexture(start_texture);

    sf::Texture startfont_texture;
    startfont_texture.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Images/Font.png");


    // Variables de blinking text
    bool show_text = true;
    sf::Clock blink_clock;
    sf::Time blink_interval = sf::seconds(0.5f);

    //Variables de audio
    sf::SoundBuffer explosion_buffer;

    explosion_buffer.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Music/explosion.wav");

    sf::Sound explosion_sound;
    explosion_sound.setBuffer(explosion_buffer);


    sf::SoundBuffer laser_buffer;
    laser_buffer.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Music/laser.wav");

    sf::Sound laser_sound;
    laser_sound.setBuffer(laser_buffer);

    sf::Music music;
    music.openFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Music/music.wav");
    music.setLoop(true);
    music.play();
    music.setVolume(30);

    // Dibujar la ventana de inicio
    while (1 == start_window.isOpen())
    {


        while (1 == start_window.pollEvent(event))
        {

            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    start_window.close();
                }
                case sf::Event::KeyPressed:
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        // Iniciar el juego
                        start_window.close();
                    }
                }
            }
        }

        start_window.clear();
        start_window.draw(start_sprite);

        // Mostrar texto cada blink_interval segundos
        if (blink_clock.getElapsedTime() > blink_interval)
        {
            show_text = !show_text;
            blink_clock.restart();
        }

        // Dibujar el texto si es visible
        if (show_text)
        {
            draw_text(0.5f * (SCREEN_WIDTH - 13.5f * BASE_SIZE), 0.5f * (SCREEN_HEIGHT - BASE_SIZE), "Presiona ENTER para comenzar", start_window, startfont_texture);
        }

        start_window.display();
        start_window.setFramerateLimit(60);
    }
    //Crear ventana de control
    sf::RenderWindow control_window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Space Invaders", sf::Style::Close);
    sf::Image controlicon_image;
    controlicon_image.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Images/icon.png");

    control_window.setIcon(controlicon_image.getSize().x, controlicon_image.getSize().y, controlicon_image.getPixelsPtr());

    control_window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));

    //Cargar texturas background
    sf::Texture control_texture;
    control_texture.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Images/ControlScreen.png");

    sf::Sprite control_sprite;
    control_sprite.setTexture(control_texture);

    sf::Texture controlfont_texture;
    controlfont_texture.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Images/Font.png");

    // Variables de blinking text
    bool showcontrol_text = true;
    sf::Clock blinkcontrol_clock;
    sf::Time blinkcontrol_interval = sf::seconds(0.5f);

    // Dibujar la ventana de control
    while (1 == control_window.isOpen())
    {
        while (1 == control_window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    control_window.close();
                }
                case sf::Event::KeyPressed:
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        // Iniciar el juego
                        control_window.close();
                    }
                }
            }
        }

        control_window.clear();
        control_window.draw(control_sprite);

        // Mostrar texto cada blink_interval segundos
        if (blinkcontrol_clock.getElapsedTime() > blinkcontrol_interval)
        {
            showcontrol_text = !showcontrol_text;
            blinkcontrol_clock.restart();
        }

        // Dinujar el texto si es visible
        if (showcontrol_text)
        {
            draw_text(0.5f * (SCREEN_WIDTH - 13.5f * BASE_SIZE), 0.5f * (SCREEN_HEIGHT - 4.5f * BASE_SIZE), "Presiona ENTER para continuar", control_window, controlfont_texture);
        }

        control_window.display();
        control_window.setFramerateLimit(60);
    }


    sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Space Invaders", sf::Style::Close);
    //Re size de la ventana
    sf::Image icongame_image;
    icongame_image.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Images/icon.png");

    window.setIcon(icongame_image.getSize().x, icongame_image.getSize().y, icongame_image.getPixelsPtr());

    window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));

    sf::Sprite background_sprite;
    sf::Sprite powerup_bar_sprite;

    sf::Texture background_texture;
    background_texture.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Images/Background.png");

    sf::Texture font_texture;
    font_texture.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Images/Font.png");

    sf::Texture powerup_bar_texture;
    powerup_bar_texture.loadFromFile("/Users/JurgenZellhuber/OneDrive - Laureate Mexico/ARQ. Y PROG. DE COMPUTADORAS/HelloSFML/Resources/Images/PowerupBar.png");

    EnemyManager enemy_manager;

    Player player;

    Ufo ufo(random_engine);

    background_sprite.setTexture(background_texture);
    powerup_bar_sprite.setTexture(powerup_bar_texture);

    previous_time = std::chrono::steady_clock::now();

    bool deadblink_text = true;
    sf::Clock deadblink_clock;
    sf::Time deadblink_interval = sf::seconds(0.3f);

    while (1 == window.isOpen())
    {
        //FPS independientes
        std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

        lag += delta_time;

        previous_time += delta_time;

        while (FRAME_DURATION <= lag)
        {
            lag -= FRAME_DURATION;

            while (1 == window.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                    {
                        window.close();
                    }
                }
            }

            //Fin del juego
            if (1 == player.get_dead_animation_over())
            {

                game_over = 1;
            }

            if (1 == enemy_manager.reached_player(player.get_y()))
            {

                player.die();
            }

            if (0 == game_over)
            {
                if (0 == enemy_manager.get_enemies().size())
                {
                    if (0 == next_level_timer)
                    {
                        next_level = 0;

                        level++;
                        next_level_timer = NEXT_LEVEL_TRANSITION;

                        player.reset();

                        enemy_manager.reset(level);

                        ufo.reset(1, random_engine);
                    }
                    else //Siguiente nivel
                    {
                        next_level = 1;

                        next_level_timer--;
                    }
                }
                else
                {
                    player.update(random_engine, enemy_manager.get_enemy_bullets(), enemy_manager.get_enemies(), ufo);

                    enemy_manager.update(random_engine);

                    ufo.update(random_engine);
                }
            }
            else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                game_over = 0;

                level = 0;

                player.reset();

                enemy_manager.reset(level);

                ufo.reset(1, random_engine);
            }

            if (FRAME_DURATION > lag)
            {
                window.clear();

                window.draw(background_sprite);

                //Cuando el jugador muere, no se muestra nada mas que la nave
                if (0 == player.get_dead())
                {
                    enemy_manager.draw(window);

                    ufo.draw(window);

                    //Duraccion de los powerups
                    if (0 < player.get_current_power())
                    {
                        powerup_bar_sprite.setColor(sf::Color(255, 255, 255));
                        powerup_bar_sprite.setPosition(SCREEN_WIDTH - powerup_bar_texture.getSize().x - 0.25f * BASE_SIZE, 0.25f * BASE_SIZE);
                        powerup_bar_sprite.setTextureRect(sf::IntRect(0, 0, powerup_bar_texture.getSize().x, BASE_SIZE));

                        window.draw(powerup_bar_sprite);

                        powerup_bar_sprite.setPosition(SCREEN_WIDTH - powerup_bar_texture.getSize().x - 0.125f * BASE_SIZE, 0.25f * BASE_SIZE);
                        //Tamaño de la barra de powerup
                        powerup_bar_sprite.setTextureRect(sf::IntRect(0.125f * BASE_SIZE, BASE_SIZE, ceil(player.get_power_timer() * static_cast<float>(powerup_bar_texture.getSize().x - 0.25f * BASE_SIZE) / POWERUP_DURATION), BASE_SIZE));

                        switch (player.get_current_power())
                        {
                            case 1:
                            {
                                powerup_bar_sprite.setColor(sf::Color(0, 146, 255));

                                break;
                            }
                            case 2:
                            {
                                powerup_bar_sprite.setColor(sf::Color(255, 0, 0));

                                break;
                            }
                            case 3:
                            {
                                powerup_bar_sprite.setColor(sf::Color(255, 219, 0));

                                break;
                            }
                            case 4:
                            {
                                powerup_bar_sprite.setColor(sf::Color(219, 0, 255));
                            }
                        }

                        window.draw(powerup_bar_sprite);
                    }
                }

                player.draw(window);

                draw_text(0.25f * BASE_SIZE, 0.25f * BASE_SIZE, "Level: " + std::to_string(level), window, font_texture);



                if (1 == game_over)
                {
                    if (deadblink_clock.getElapsedTime() > deadblink_interval)
                    {
                        deadblink_text = !deadblink_text;
                        deadblink_clock.restart();
                    }
                    draw_text(0.5f * (SCREEN_WIDTH - 5.5f * BASE_SIZE), 0.5f * (SCREEN_HEIGHT - BASE_SIZE), "GAME OVER!", window, font_texture);
                    if (deadblink_text)
                    {
                        draw_text(0.5f * (SCREEN_WIDTH - 18.5f * BASE_SIZE), 0.5f * (SCREEN_HEIGHT - BASE_SIZE) + 1.5f * BASE_SIZE, "CONTINUE?: ENTER para volver a empezar", window, font_texture);
                    }
                }
                else if (1 == next_level)
                {
                    draw_text(0.5f * (SCREEN_WIDTH - 5.5f * BASE_SIZE), 0.5f * (SCREEN_HEIGHT - BASE_SIZE), "Next level!", window, font_texture);
                }

                window.display();
            }
        }
    }
}
