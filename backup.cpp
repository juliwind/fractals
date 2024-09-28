#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <SFML/Graphics.hpp>
#include <cmath>

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

void drawFractal(sf::RenderWindow& window, unsigned int width, unsigned int height, double c_re, double c_im, int max_iterations, double minRe, double maxRe, double minIm, double maxIm) {
    vector<vector<int> > grid(width, vector<int>(height));
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            double z_re = minRe + ((double)x / (width - 1)) * (maxRe - minRe);
            double z_im = maxIm - ((double)y / (height - 1)) * (maxIm - minIm);
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

void zoom(double zoomFactor, sf::RenderWindow& window, unsigned int width, unsigned int height, double c_re, double c_im, int& max_iterations,
          double& minRe, double& maxRe, double& minIm, double& maxIm, bool visualizationActive, double initialScale, int initial_max_iterations) {
    // Zentrum des Bildschirms in komplexe Koordinaten umrechnen
    double centerRe = (minRe + maxRe) / 2.0;
    double centerIm = (minIm + maxIm) / 2.0;

    // Neuen Bereich berechnen
    double newWidth = (maxRe - minRe) * zoomFactor;
    double newHeight = (maxIm - minIm) * zoomFactor;

    minRe = centerRe - newWidth / 2.0;
    maxRe = centerRe + newWidth / 2.0;
    minIm = centerIm - newHeight / 2.0;
    maxIm = centerIm + newHeight / 2.0;

    // Aktuellen Maßstab berechnen
    double currentScale = maxRe - minRe;

    // max_iterations dynamisch anpassen
    max_iterations = initial_max_iterations + static_cast<int>(log(initialScale / currentScale) * 20);
    if (max_iterations < initial_max_iterations)
        max_iterations = initial_max_iterations; // Mindestwert sicherstellen

    // Optional: Obergrenze für max_iterations setzen
    if (max_iterations > 1000)
        max_iterations = 1000;

    // Fraktal neu zeichnen
    if (visualizationActive) {
        drawFractal(window, width, height, c_re, c_im, max_iterations, minRe, maxRe, minIm, maxIm);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Fractals!!");
    sf::Vector2u size = window.getSize();
    unsigned int width = size.x;
    unsigned int height = size.y;

    double c_re = -0.7;
    double c_im = 0.27015;
    int max_iterations = 100;

    // Bereich der komplexen Ebene
    double minRe = -2.0;
    double maxRe = 2.0;
    double minIm = -2.0;
    double maxIm = 2.0;

    // Initialen Maßstab und max_iterations speichern
    double initialScale = maxRe - minRe;
    int initial_max_iterations = max_iterations;

    sf::Font font;
    if (!font.loadFromFile("Courier_New.ttf")) {
        cout << "Can't load font" << endl;
        return -1;
    }
    sf::Text text;
    text.setFont(font);
    text.setString("Press ENTER to start and end the visualization.\nTo change the c-values, press SPACE and use the console.\nUse '+' and '-' to zoom in and out.");
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

    bool visualizationActive = false;
    bool isDragging = false;
    sf::Vector2i lastMousePos;

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Tastatureingaben
            if (event.type == sf::Event::KeyPressed) {
                // Zoom In mit "+" oder "W"
                if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::W) {
                    // Zoom In
                    double zoomFactor = 0.9;
                    zoom(zoomFactor, window, width, height, c_re, c_im, max_iterations, minRe, maxRe, minIm, maxIm, visualizationActive, initialScale, initial_max_iterations);
                }
                // Zoom Out mit "-" oder "S"
                else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::S) {
                    // Zoom Out
                    double zoomFactor = 1.1;
                    zoom(zoomFactor, window, width, height, c_re, c_im, max_iterations, minRe, maxRe, minIm, maxIm, visualizationActive, initialScale, initial_max_iterations);
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    visualizationActive = !visualizationActive;

                    if (visualizationActive) {
                        drawFractal(window, width, height, c_re, c_im, max_iterations, minRe, maxRe, minIm, maxIm);
                    } else {
                        window.clear(sf::Color::Black);
                        window.draw(text);
                        window.display();
                    }
                } else if (event.key.code == sf::Keyboard::Space) {
                    // Eingabevalidierung für c_re
                    while (true) {
                        cout << "Enter a new value for the real part of c: ";
                        if (cin >> c_re) {
                            break; // Gültige Eingabe erhalten
                        } else {
                            cout << "Invalid input. Please enter a numeric value." << endl;
                            cin.clear(); // Fehlerzustand zurücksetzen
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Eingabepuffer leeren
                        }
                    }

                    // Eingabevalidierung für c_im
                    while (true) {
                        cout << "Enter a new value for the imaginary part of c: ";
                        if (cin >> c_im) {
                            break; // Gültige Eingabe erhalten
                        } else {
                            cout << "Invalid input. Please enter a numeric value." << endl;
                            cin.clear(); // Fehlerzustand zurücksetzen
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Eingabepuffer leeren
                        }
                    }

                    if (visualizationActive) {
                        drawFractal(window, width, height, c_re, c_im, max_iterations, minRe, maxRe, minIm, maxIm);
                    }
                }
            }

            // Maus-Events für Pan
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isDragging = true;
                    lastMousePos = sf::Mouse::getPosition(window);
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isDragging = false;
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                if (isDragging && visualizationActive) {
                    sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                    sf::Vector2i delta = lastMousePos - currentMousePos;

                    // Umrechnung der Pixelverschiebung in komplexe Verschiebung
                    double deltaRe = ((double)delta.x / (width - 1)) * (maxRe - minRe);
                    double deltaIm = ((double)delta.y / (height - 1)) * (maxIm - minIm);

                    minRe += deltaRe;
                    maxRe += deltaRe;
                    minIm -= deltaIm;
                    maxIm -= deltaIm;

                    lastMousePos = currentMousePos;

                    // Fraktal neu zeichnen
                    drawFractal(window, width, height, c_re, c_im, max_iterations, minRe, maxRe, minIm, maxIm);
                }
            }
        }
    }

    return 0;
}
