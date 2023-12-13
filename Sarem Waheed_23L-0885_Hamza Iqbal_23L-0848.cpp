#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace std;
int player1Score = 0;
int player2Score = 0;

enum class Player {
    Player1,
    Player2
};

enum class GameState {
    StartScreen,
    Playing
};

int totalSquares = 25, placedsquare = 0;

// Declarations for white squares and colored rectangles (moved outside main)
std::vector<sf::RectangleShape> whiteSquares;
std::vector<sf::RectangleShape> coloredRectangles;
std::vector<sf::RectangleShape> blackSquares;

// Function to check for completed squares
void checkForCompletedSquares(Player& currentPlayer) {
    std::vector<sf::RectangleShape> completedSquaresToRemove;
    bool completed = false;
    Player savestate;
    for (const auto& whiteSquare : whiteSquares) {
        bool topLeft = false;
        bool topRight = false;
        bool bottomLeft = false;
        bool bottomRight = false;

        // Check each corner of the white square
        sf::Vector2f whiteSquareLeft = whiteSquare.getPosition() + sf::Vector2f(0, 70);
        sf::Vector2f whiteSquareTop = whiteSquare.getPosition() + sf::Vector2f(70, 0);
        sf::Vector2f whiteSquareBottom = whiteSquare.getPosition() + sf::Vector2f(70, 140);
        sf::Vector2f whiteSquareRight = whiteSquare.getPosition() + sf::Vector2f(140, 70);

        for (const auto& coloredRect : coloredRectangles) {
            // Check if each corner is inside a colored rectangle
            if (coloredRect.getGlobalBounds().contains(whiteSquareLeft)) {
                topLeft = true;
            }
            if (coloredRect.getGlobalBounds().contains(whiteSquareTop)) {
                topRight = true;
            }
            if (coloredRect.getGlobalBounds().contains(whiteSquareBottom)) {
                bottomLeft = true;
            }
            if (coloredRect.getGlobalBounds().contains(whiteSquareRight)) {
                bottomRight = true;
            }
        }

        if (topLeft && topRight && bottomLeft && bottomRight) {
            Player squarePlayer = (currentPlayer == Player::Player1) ? Player::Player2 : Player::Player1;
           
            // Create a black square on top of the white square with the player's color
            sf::RectangleShape blackSquare(sf::Vector2f(140, 140)); // Adjust size as needed
            blackSquare.setPosition(whiteSquare.getPosition());
            if (squarePlayer == Player::Player1) {
                blackSquare.setFillColor(sf::Color(66, 113, 222)); // Set the color for Player 1
                player1Score++;  // Increment Player 1 score
            }
            else {
                blackSquare.setFillColor(sf::Color(254, 34, 44)); // Set the color for Player 2
                player2Score++;  // Increment Player 2 score
            }

            blackSquares.push_back(blackSquare);
            completedSquaresToRemove.push_back(whiteSquare);
            placedsquare++;
            savestate = squarePlayer;
            completed = true;
        }
    }
    // Remove completed white squares from the vector
    for (const auto& completedSquare : completedSquaresToRemove) {
        whiteSquares.erase(
            std::remove_if(
                whiteSquares.begin(),
                whiteSquares.end(),
                [&](const sf::RectangleShape& square) {
                    return square.getPosition() == completedSquare.getPosition();
                }
            ),
            whiteSquares.end()
        );
    }
    if (completed == true) {
        currentPlayer = savestate;
    }
}

GameState gameState = GameState::StartScreen;
int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Dot and Boxes", sf::Style::Default /*|| sf::Style::Fullscreen*/);  // Displays render window
    sf::RectangleShape board(sf::Vector2f(900.0f, 900.0f)); // Create white board
    sf::RectangleShape player(sf::Vector2f(550.0f, 900.0f)); // Create 2 player profiles board
    sf::RectangleShape betweenshapes(sf::Vector2f(550.0f, 125.0f)); // Creates Space between 2 player profiles board

    board.setFillColor(sf::Color::White);  // Give the board white color
    player.setFillColor(sf::Color::White);  // Give the player white color
    betweenshapes.setFillColor(sf::Color(15, 135, 255));  // Five between player profiles silver color

    board.setPosition((window.getSize().y - board.getSize().y) / 2 + 100, (window.getSize().y - board.getSize().y) / 2);  // Makes the board on the left and centered
    player.setPosition(1920 - 825 + 100, (window.getSize().y - player.getSize().y) / 2);  // Makes the player on the right and centered
    betweenshapes.setPosition(1920 - 825 + 100, (window.getSize().y - betweenshapes.getSize().y) / 2);  // Makes the space on the right and centered between player

    sf::Texture startscreen;
    startscreen.loadFromFile("start screen.jpg");
    sf::IntRect textureRect(0, 0, 1920, 1080);
    startscreen.setRepeated(true);
    sf::Sprite spritess(startscreen , textureRect); // Sprite to display the start screen
    sf::Sprite sprite(startscreen, textureRect);  // Sprite used to display the silver background

    // Loads the player 1 pic
    sf::Texture player1pic;
    player1pic.loadFromFile("Player 1 pic.png");
    sf::Sprite sprite1(player1pic);  // Sprite used to display the player 1 pic
    sprite1.setPosition(1230.0f, (window.getSize().y - board.getSize().y) / 2 + 37);
    sprite1.setScale(sf::Vector2f(0.168f, 0.168f));

    // Loads the player 2 pic
    sf::Texture player2pic;
    player2pic.loadFromFile("Player 2 pic.jpg");
    sf::Sprite sprite2(player2pic);  // Sprite used to display the player 1 pic
    sprite2.setPosition(1230.0f, 640);
    sprite2.setScale(sf::Vector2f(0.7f, 0.7f));

    // Player text
    sf::Font playertitle;
    if (!playertitle.loadFromFile("SupernormaleEigSta W00 Bold.ttf")) {
        // Handle font loading error
        return 1;
    }

    sf::Font playerscore;
    if (!playerscore.loadFromFile("ROCK.TTF")) {
        // Handle font loading error
        return 1;
    }

    // Title screen text
    sf::Text startText;
    startText.setFont(playertitle);
    startText.setString("Dot & Boxes");
    startText.setCharacterSize(100);
    startText.setFillColor(sf::Color::White);  // Set the fill color
    startText.setOutlineColor(sf::Color::Black);
    startText.setOutlineThickness(10.0f);
    sf::FloatRect textBounds = startText.getLocalBounds();
    startText.setPosition((window.getSize().x - textBounds.width) / 2, (window.getSize().y - textBounds.height) / 2);

    sf::Text entertext;
    entertext.setFont(playerscore);
    entertext.setString("Press Enter to Start");
    entertext.setCharacterSize(50);
    entertext.setFillColor(sf::Color::Black);  // Set the fill color
    //startText.setOutlineColor(sf::Color::Black);
    //startText.setOutlineThickness(10.0f);
    sf::FloatRect textBounds1 = entertext.getLocalBounds();
    entertext.setPosition((window.getSize().x - textBounds1.width) / 2, (window.getSize().y - board.getSize().y) / 2 + 520);  // Set the position of the text

    //  Player 1 text
    sf::Text player1text;
    player1text.setFont(playertitle);  // Set the font
    player1text.setString("Player 1");  // Set the text string
    player1text.setCharacterSize(30);  // Set the character size
    player1text.setFillColor(sf::Color(66, 113, 222));  // Set the fill color
    player1text.setPosition(1480.0f, (window.getSize().y - board.getSize().y) / 2 + 47);  // Set the position of the text

    //  Player 2 text
    sf::Text player2text;
    player2text.setFont(playertitle);  // Set the font
    player2text.setString("Player 2");  // Set the text string
    player2text.setCharacterSize(32);  // Set the character size
    player2text.setFillColor(sf::Color(254, 34, 44));  // Set the fill color
    player2text.setPosition(1480.0f, (window.getSize().y - board.getSize().y) / 2 + 560);  // Set the position of the text

    // Score text for player 1
    sf::Text player1scores;
    player1scores.setFont(playerscore);  // Set the font
    player1scores.setString("Score: ");  // Set the text string
    player1scores.setCharacterSize(30);  // Set the character size
    player1scores.setFillColor(sf::Color(66, 113, 222));  // Set the fill color
    player1scores.setPosition(1480.0f, (window.getSize().y - board.getSize().y) / 2 + 100);  // Set the position of the text

    // Score text for player 2
    sf::Text player2scores;
    player2scores.setFont(playerscore);  // Set the font
    player2scores.setString("Score: ");  // Set the text string
    player2scores.setCharacterSize(30);  // Set the character size
    player2scores.setFillColor(sf::Color(254, 34, 44));  // Set the fill color
    player2scores.setPosition(1480.0f, (window.getSize().y - board.getSize().y) / 2 + 613);  // Set the position of the text

    // No. of rows, columns & set cellsize 
    const int rows = 6;
    const int cols = 6;
    const float cellSize = 140.0f;
    const float dotRadius = 20.0f;

    // Set the position of the grid
    const float gridX = (window.getSize().y - board.getSize().y) / 2 + 200;
    const float gridY = (window.getSize().y - board.getSize().y) / 2 + 100;

    // Create dots at grid intersections
    std::vector<sf::CircleShape> dots;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            sf::CircleShape dot(dotRadius);
            dot.setPosition(gridX + i * cellSize - dot.getRadius(), gridY + j * cellSize - dot.getRadius());
            dot.setFillColor(sf::Color::Black);
            dots.push_back(dot);
        }
    }

    // Initial player turn
    Player currentPlayer = Player::Player1;

    // Vector to store white rectangles
    std::vector<sf::RectangleShape> whiteRectangles;
    sf::Vector2f whiteRectSize(2.0f, 100.0f); // Set the size you want
    sf::Vector2f initialwhiteRectPosition((window.getSize().y - board.getSize().y) / 2 + 200, (window.getSize().y - board.getSize().y) / 2 + 100); // Set the initial position you want

    // Create white rectangles horizaontally
    for (int i = 0; i < rows - 1; ++i) {
        for (int j = 0; j < cols; ++j) {
            sf::RectangleShape whiteRect(sf::Vector2f(cellSize, 20));

            whiteRect.setPosition(initialwhiteRectPosition.x + i * cellSize + cellSize / 2.0f, initialwhiteRectPosition.y + j * cellSize);
            whiteRect.setFillColor(sf::Color(211, 211, 211));
            whiteRect.setOrigin(cellSize / 2.0f, 10.0f); // Set the origin to the center
            whiteRectangles.push_back(whiteRect);

            if (j != 5) {
                // Create white square in the middle of the white rectangle
                sf::RectangleShape whiteSquare(sf::Vector2f(140, 140)); // Adjust size as needed
                whiteSquare.setPosition(initialwhiteRectPosition.x + i * cellSize, initialwhiteRectPosition.y + j * cellSize);
                whiteSquare.setFillColor(sf::Color::White); // Set the color
                whiteSquares.push_back(whiteSquare);
            }
        }
    }

    // Create white rectangles vertically 
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols - 1; ++j) {
            sf::RectangleShape whiteRect(sf::Vector2f(20, cellSize));
            
            whiteRect.setPosition(initialwhiteRectPosition.x + i * cellSize, initialwhiteRectPosition.y + j * cellSize + cellSize / 2.0f);
            whiteRect.setFillColor(sf::Color(211, 211, 211));
            whiteRect.setOrigin(10.0f, cellSize / 2.0f); // Set the origin to the center
            whiteRectangles.push_back(whiteRect);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Check for keyboard press event
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    // Transition to playing state when Enter is pressed
                    gameState = GameState::Playing;
                }
            }
            // Check for mouse press event
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Check if the mouse is on top of any white rectangle
                for (const auto& whiteRect : whiteRectangles) {
                    if (whiteRect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        // Check if there is already a colored rectangle at this position
                        bool canPlaceColoredRect = true;
                        for (const auto& coloredRect : coloredRectangles) {
                            if (coloredRect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                                canPlaceColoredRect = false;
                                break;
                            }
                        }
                       
                        // If there is no colored rectangle at this position, proceed
                        if (canPlaceColoredRect) {
                            // Create a colored rectangle on top of the clicked white rectangle
                            sf::RectangleShape coloredRect(whiteRect.getSize());
                            coloredRect.setPosition(whiteRect.getPosition());

                            // Set the color based on the current player
                            if (currentPlayer == Player::Player1) {
                                coloredRect.setFillColor(sf::Color(66, 113, 222)); // Blue
                            }
                            else {
                                coloredRect.setFillColor(sf::Color(254, 34, 44)); // Red
                            }

                            // Set the origin to the center
                            coloredRect.setOrigin(coloredRect.getSize().x / 2.0f, coloredRect.getSize().y / 2.0f);

                            // Switch the player turn for the next move
                            currentPlayer = (currentPlayer == Player::Player1) ? Player::Player2 : Player::Player1;

                            // Add the colored rectangle to a vector (for drawing purposes)
                            coloredRectangles.push_back(coloredRect);

                            // After placing a colored rectangle, check for completed squares
                            checkForCompletedSquares(currentPlayer);

                        }
                    }       
                }
            }

            // Score counter for player 1
            sf::Text player1score;
            player1score.setFont(playertitle);
            player1score.setCharacterSize(100);  // Set the character size
            player1score.setString( std::to_string(player1Score));
            player1score.setFillColor(sf::Color(66, 113, 222));  // Set the fill color
            // Calculate the width of the text
            float player1scoreWidth = player1score.getGlobalBounds().width;
            // Position the text in the center
            player1score.setPosition(1545.0f - player1scoreWidth / 2.0f, (window.getSize().y - board.getSize().y) / 2 + 190);

            // Score counter for player 2
            sf::Text player2score;
            player2score.setFont(playertitle);
            player2score.setCharacterSize(100);  // Set the character size
            player2score.setString(std::to_string(player2Score));
            player2score.setFillColor(sf::Color(254, 34, 44));  // Set the fill color
            // Calculate the width of the text
            float player2scoreWidth = player2score.getGlobalBounds().width;
            // Position the text in the center
            player2score.setPosition(1545.0f - player2scoreWidth / 2.0f, (window.getSize().y - board.getSize().y) / 2 + 710);

            // Check for the winner after the game loop
            sf::Text winnerText;
            winnerText.setFont(playertitle);
            winnerText.setCharacterSize(100);
            winnerText.setFillColor(sf::Color::White);
            winnerText.setOutlineColor(sf::Color::Black);
            winnerText.setOutlineThickness(10.0f);
            sf::FloatRect textBounds4 = startText.getLocalBounds();
            winnerText.setPosition((window.getSize().x - textBounds4.width) / 2 -100, (window.getSize().y - textBounds4.height) / 2);
            if (player1Score > player2Score) {
                winnerText.setString("Player 1 wins!");
            }
            else if (player1Score < player2Score) {
                winnerText.setString("Player 2 wins!");
            }
            else {
                winnerText.setString("It's a draw!");
            }

            window.clear();  // Clear the window
            if (gameState == GameState::StartScreen) {
                window.draw(spritess);
                window.draw(startText);
                window.draw(entertext);
            }
            else if (gameState == GameState::Playing) {
                window.draw(sprite);  // Draws the silver background on screen
                window.draw(player);  // Draws player board
                window.draw(board);  // Draws white board
                window.draw(betweenshapes);  // Draws player space between player board
                window.draw(sprite1);  // Draws player 1 pic
                window.draw(sprite2);  // Draws player 2 pic
                window.draw(player1text);  // Draws PLAYER 1 text
                window.draw(player1scores);  // Draws score text for player 1
                window.draw(player1score); // Draws player 1 score counter
                window.draw(player2text);  // Draws PLAYER 2 text
                window.draw(player2score);  // Draws score text for player 2
                window.draw(player2scores);  // Draws score counter for player 2

                // Draw white squares
                for (const auto& whiteSquare : whiteSquares) {
                    window.draw(whiteSquare);
                }

                // Draw black squares
                for (const auto& blackSquare : blackSquares) {
                    window.draw(blackSquare);
                }
                // Draw white rectangles
                for (const auto& whiteRect : whiteRectangles) {
                    window.draw(whiteRect);
                }

                // Draw colored rectangles
                for (const auto& coloredRect : coloredRectangles) {
                    window.draw(coloredRect);
                }

                // Draw dots
                for (const auto& dot : dots) {
                    window.draw(dot);
                }
                if (totalSquares == placedsquare) {
                    window.draw(winnerText);
                }
            }

            window.display();  // Display the drawn objects
        }

    }
    return 0;
}

