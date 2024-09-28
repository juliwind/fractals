#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <SFML/Graphics.hpp>

using namespace std;

int calc(int max_iterations, double z_re, double z_im, double c_re, double c_im) {
    int iterations = 0;
    while (iterations < max_iterations) {
        double new_re = z_re * z_re - z_im * z_im + c_re;
        double new_im = 2.0 * z_im * z_re + c_im;
        z_re = new_re;
        z_im = new_im;
        if ((z_re * z_re + z_im * z_im) > 4.0) {
            break;
        }
        iterations++;
    }
    return iterations;
}


sf::Color getColor(int iterations, int max_iterations) {
    if (iterations == max_iterations) {
        return sf::Color::Black;
    } else {
        float t = (float)iterations / (float)max_iterations;
        sf::Uint8 r = (sf::Uint8)(9 * (1 - t) * t * t * t * 255);
        sf::Uint8 g = (sf::Uint8)(15 * (1 - t) * (1 - t) * t * t * 255);
        sf::Uint8 b = (sf::Uint8)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
        return sf::Color(r, g, b);
    }
}

void drawJulia(sf::RenderWindow& window, int width, int height, int max_iterations, double c_re, double c_im, double min_re, double max_re, double min_im, double max_im) {
    vector<vector<int> > grid(width, vector<int>(height));
    double x_size = max_re - min_re;
    double y_size = max_im - min_im;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            double z_re = ((double)x / (width-1)) * x_size + min_re;
            double z_im = ((double)y / (height-1)) * y_size - max_im;
            int px_value = calc(max_iterations, z_re, z_im, c_re, c_im);
            grid[x][y] = px_value;
        }
    }
    window.clear(sf::Color::Black);
    sf::VertexArray point_system(sf::Points);
    for (unsigned int i = 0; i < width; i++) {
        for (unsigned int j = 0; j < height; j++) {
            sf::Color color = getColor(grid[i][j], max_iterations);
            sf::Vertex point(sf::Vector2f((float)i, (float)j), color);
            point_system.append(point);
        }
    }
    window.draw(point_system);
    window.display();
}

void drawMandelbrot(sf::RenderWindow& window, int width, int height, int max_iterations, double min_re, double max_re, double min_im, double max_im) {
    vector<vector<int> > grid(width, vector<int>(height));
    double x_size = max_re - min_re;
    double y_size = max_im - min_im;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            double c_re = ((double)x / (width-1)) * x_size + min_re;
            double c_im = ((double)y / (height-1)) * y_size - max_im;
            int px_value = calc(max_iterations, c_re, c_im, c_re, c_im);
            grid[x][y] = px_value;
        }
    }
    window.clear(sf::Color::Black);
    sf::VertexArray point_system(sf::Points);
    for (unsigned int i = 0; i < width; i++) {
        for (unsigned int j = 0; j < height; j++) {
            sf::Color color = getColor(grid[i][j], max_iterations);
            sf::Vertex point(sf::Vector2f((float)i, (float)j), color);
            point_system.append(point);
        }
    }
    window.draw(point_system);
    window.display();
}


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Fractal Visualizer");

    sf::Vector2u size = window.getSize();
    int width = size.x;
    int height = size.y;

    double c_re = -0.7;
    double c_im = 0.27015;

    double min_re = -2.0;
    double max_re = 2.0;
    double min_im = -2.0;
    double max_im = 2.0;
    double initial_scale = max_re - min_re;

    int initial_max_iterations = 100;
    int max_iterations = initial_max_iterations;

    bool is_visualization = false;
    bool is_mandelbrot = false;

    bool mouse_dragging = false;
    sf::Vector2i last_mouse_pos;

    sf::Font font;
    if (!font.loadFromFile("Courier_New.ttf")) {
        cout << "Can't load font" << endl;
        return -1;
    }
    sf::Text text;
    text.setFont(font);
    text.setString("Press ENTER to start and end the visualization.\n To change the c-values, press SPACE and use the console.\nTo switch between the Mandelbrot Set and the Julia Set,\npress 's'.");
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Green);
    text.setStyle(sf::Text::Bold);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(window.getSize().x / 2.0f,
                                  window.getSize().y / 2.0f));

    window.clear(sf::Color::Black);
    window.draw(text);
    window.display();

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    is_visualization = !is_visualization;

                    if (is_visualization) {
                        if (is_mandelbrot) {
                            drawMandelbrot(window, width, height, max_iterations, min_re, max_re, min_im, max_im);
                        }
                        else {
                            drawJulia(window, width, height, max_iterations, c_re, c_im, min_re, max_re, min_im, max_im);
                        }
                    } 
                    else {
                        window.clear(sf::Color::Black);
                        window.draw(text);
                        window.display();
                    }
                }
                else if (event.key.code == sf::Keyboard::Space) {
                    while (true) {
                        cout << "Enter a new value for the real part of c: ";
                        if (cin >> c_re) {
                            break;
                        } else {
                            cout << "Invalid input. Please enter a numeric value." << endl;
                            cin.clear();
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    }

                    while (true) {
                        cout << "Enter a new value for the imaginary part of c: ";
                        if (cin >> c_im) {
                            break;
                        } else {
                            cout << "Invalid input. Please enter a numeric value." << endl;
                            cin.clear();
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    }

                    if (is_visualization) {
                        if (is_mandelbrot) {
                            drawMandelbrot(window, width, height, max_iterations, min_re, max_re, min_im, max_im);
                        }
                        else {
                            drawJulia(window, width, height, max_iterations, c_re, c_im, min_re, max_re, min_im, max_im);
                        }
                    }
                }
                else if(event.key.code == sf::Keyboard::S) {
                    is_mandelbrot = !is_mandelbrot;
                }
                else if(event.key.code == sf::Keyboard::Add) {
                    if(is_visualization) {
                        double zoom_factor = 0.9;
                        double center_re = (max_re - min_re) / 2;
                        double center_im = (max_im - min_im) / 2;
                        double new_center_dist_re = ((max_re - min_re) * zoom_factor) / 2;
                        double new_center_dist_im = ((max_im - min_im) * zoom_factor) / 2;
                        
                        min_re = center_re - new_center_dist_re;
                        max_re = center_re + new_center_dist_re;
                        min_im = center_im - new_center_dist_im;
                        max_im = center_im + new_center_dist_im;

                        double currentScale = max_re - min_re;
                        max_iterations = initial_max_iterations + static_cast<int>(log(initial_scale / currentScale) * 20);
                        if (max_iterations < initial_max_iterations) {
                            max_iterations = initial_max_iterations;
                        }
                        if(is_mandelbrot) {
                            drawMandelbrot(window, width, height, max_iterations, min_re, max_re, min_im, max_im);
                        }
                        if(!is_mandelbrot) {
                            drawJulia(window, width, height, max_iterations, c_re, c_im, min_re, max_re, min_im, max_im);
                        }
                    }
                    
                }
                else if(event.key.code == sf::Keyboard::Subtract) {
                    if(is_visualization) {
                        double zoom_factor = 1.1;
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    mouse_dragging = true;
                    last_mouse_pos = sf::Mouse::getPosition(window);
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    mouse_dragging = false;
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                if(mouse_dragging && is_visualization) {
                    sf::Vector2i new_mouse_pos = sf::Mouse::getPosition(window);
                    sf::Vector2i delta = new_mouse_pos - last_mouse_pos;
                    double x_size = max_re - min_re;
                    double y_size = max_im - min_im;
                    double delta_re = ((double)delta.x / (width-1)) * x_size;
                    double delta_im = ((double)delta.y / (height-1)) * y_size;

                    min_re -= delta_re;
                    max_re -= delta_re;
                    min_im += delta_im;
                    max_im += delta_im;

                    last_mouse_pos = new_mouse_pos;

                    if(is_mandelbrot) {
                        drawMandelbrot(window, width, height, max_iterations, min_re, max_re, min_im, max_im);
                    }
                    if(!is_mandelbrot) {
                        drawJulia(window, width, height, max_iterations, c_re, c_im, min_re, max_re, min_im, max_im);
                    }
                }
            }
        }
    }

    return 0;
}