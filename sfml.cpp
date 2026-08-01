#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;

const float START_X = 50.f;
const float AXIS_Y = 300.f;
const float BIT_WIDTH = 80.f;
const float HIGH_Y = 250.f;
const float LOW_Y = 350.f;

const sf::Color WAVE_COLOR = sf::Color::Red;

void drawAxis(sf::RenderWindow &window){
    sf::Vertex axis[2];

    axis[0].position = {START_X, AXIS_Y};
    axis[0].color = sf::Color::Black;

    axis[1].position = {950.f, AXIS_Y};
    axis[1].color = sf::Color::Black;

    window.draw(axis, 2, sf::PrimitiveType::Lines);
}

void drawGrid(sf::RenderWindow &window, int n){
    for (int i = 0; i <= n; i++)
    {
        sf::Vertex line[2];

        line[0].position = {START_X + i * BIT_WIDTH, HIGH_Y};
        line[0].color = sf::Color(180, 180, 180);

        line[1].position = {START_X + i * BIT_WIDTH, LOW_Y};
        line[1].color = sf::Color(180, 180, 180);

        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

void drawHorizontalLine(sf::RenderWindow &window, float x1, float x2, float y){
    sf::Vertex line[2];

    line[0].position = {x1, y};
    line[0].color = WAVE_COLOR;

    line[1].position = {x2, y};
    line[1].color = WAVE_COLOR;

    window.draw(line, 2, sf::PrimitiveType::LineStrip);
}

void drawHalfHorizontalLine(sf::RenderWindow &window, float x1, float x2, float y){
    sf::Vertex line[2];

    line[0].position = {x1, y};
    line[0].color = WAVE_COLOR;

    line[1].position = {x2, y};
    line[1].color = WAVE_COLOR;

    window.draw(line, 2, sf::PrimitiveType::LineStrip);
}

void drawTransition(sf::RenderWindow &window, float x, float y1, float y2){
    sf::Vertex line[2];

    line[0].position = {x, y1};
    line[0].color = WAVE_COLOR;

    line[1].position = {x, y2};
    line[1].color = WAVE_COLOR;

    window.draw(line, 2, sf::PrimitiveType::LineStrip);
}


void Unipolar_NRZ(sf::RenderWindow &window, string data){
    for(int i=0 ; i<data.length() ; i++){

        float y;

        if(data[i]=='1') y = HIGH_Y;
        else y = AXIS_Y;

        drawHorizontalLine(window, START_X + i * BIT_WIDTH, START_X + (i + 1) * BIT_WIDTH, y);

        if(i < data.length()-1){
            float currentY;
            float nextY;

            if(data[i]=='1') currentY = HIGH_Y;
            else currentY = AXIS_Y;

            if(data[i+1]=='1') nextY = HIGH_Y;
            else nextY = AXIS_Y;

            if(currentY != nextY) drawTransition(window,START_X + (i + 1) * BIT_WIDTH,currentY,nextY);
        }
    }
}

void Polar_NRZ(sf::RenderWindow &window, string data){
    for(int i=0 ; i<data.length() ; i++){

        float y;

        if(data[i]=='1') y = LOW_Y;
        else y = HIGH_Y;

        drawHorizontalLine(window, START_X + i * BIT_WIDTH, START_X + (i + 1) * BIT_WIDTH, y);

        if(i < data.length()-1){
            float currentY;
            float nextY;

            if(data[i]=='1') currentY = LOW_Y;
            else currentY = HIGH_Y;

            if(data[i+1]=='1') nextY = LOW_Y;
            else nextY = HIGH_Y;

            if(currentY != nextY) drawTransition(window,START_X + (i + 1) * BIT_WIDTH,currentY,nextY);
        }
    }
}

void NRZ_Inverted(sf::RenderWindow &window, string data){
    char level = 'H';

    for(int i=0 ; i<data.length() ; i++){
        float currentY;

        if(level=='H') currentY = HIGH_Y;
        else currentY = LOW_Y;

        if(data[i]=='1'){
            if(level=='H'){
                level = 'L';
                currentY = LOW_Y;
            }
            else{
                level = 'H';
                currentY = HIGH_Y;
            }

            float previousY;

            if(currentY==HIGH_Y) previousY = LOW_Y;
            else previousY = HIGH_Y;

            drawTransition(window,START_X + i * BIT_WIDTH,previousY,currentY);
        }

        drawHorizontalLine(window,START_X + i * BIT_WIDTH,START_X + (i+1) * BIT_WIDTH,currentY);
    }
}

void Bipolar_Encoding(sf::RenderWindow &window, string data){
    char prev = 'L';

    for(int i=0 ; i<data.length() ; i++){
        float x = START_X + i * BIT_WIDTH;
        float mid = x + BIT_WIDTH/2;

        if(data[i]=='1'){
            if(prev=='L'){
                drawHalfHorizontalLine(window, x, mid, HIGH_Y);
                drawTransition(window, mid, HIGH_Y, LOW_Y);
                drawHalfHorizontalLine(window, mid, x + BIT_WIDTH, LOW_Y);

                prev = 'H';
            }
            else{
                drawHalfHorizontalLine(window, x, mid, LOW_Y);
                drawTransition(window, mid, LOW_Y, HIGH_Y);
                drawHalfHorizontalLine(window, mid, x + BIT_WIDTH, HIGH_Y);

                prev = 'L';
            }
        }
        else{
            drawHorizontalLine(window, x, x + BIT_WIDTH, AXIS_Y);
        }
    }
}

void Manchester_Encoding(sf::RenderWindow &window, string data){
    for(int i=0 ; i<data.length() ; i++){
        float x = START_X + i * BIT_WIDTH;

        if(data[i]=='1'){
            drawHalfHorizontalLine(window, x, x + BIT_WIDTH/2, LOW_Y);
            drawTransition(window, x + BIT_WIDTH/2, LOW_Y, HIGH_Y);
            drawHalfHorizontalLine(window, x + BIT_WIDTH/2, x + BIT_WIDTH, HIGH_Y);
        }
        else{
            drawHalfHorizontalLine(window, x, x + BIT_WIDTH/2, HIGH_Y);
            drawTransition(window, x + BIT_WIDTH/2, HIGH_Y, LOW_Y);
            drawHalfHorizontalLine(window, x + BIT_WIDTH/2, x + BIT_WIDTH, LOW_Y);
        }
    }
}

void Differential_Manchester_Encoding(sf::RenderWindow &window, string data){
    char prev = 'Z';

    for(int i=0 ; i<data.length() ; i++){
        float x = START_X + i * BIT_WIDTH;
        float mid = x + BIT_WIDTH/2;

        if(data[i]=='0'){
            if(prev=='S'){
                drawHalfHorizontalLine(window, x, mid, LOW_Y);
                drawTransition(window, mid, LOW_Y, HIGH_Y);
                drawHalfHorizontalLine(window, mid, x + BIT_WIDTH, HIGH_Y);
            }
            else{
                drawHalfHorizontalLine(window, x, mid, HIGH_Y);
                drawTransition(window, mid, HIGH_Y, LOW_Y);
                drawHalfHorizontalLine(window, mid, x + BIT_WIDTH, LOW_Y);
            }
        }
        else{
            if(prev=='Z'){
                prev = 'S';

                drawHalfHorizontalLine(window, x, mid, LOW_Y);
                drawTransition(window, mid, LOW_Y, HIGH_Y);
                drawHalfHorizontalLine(window, mid, x + BIT_WIDTH, HIGH_Y);
            }
            else{
                prev = 'Z';

                drawHalfHorizontalLine(window, x, mid, HIGH_Y);
                drawTransition(window, mid, HIGH_Y, LOW_Y);
                drawHalfHorizontalLine(window, mid, x + BIT_WIDTH, LOW_Y);
            }
        }
    }
}

void AMI(sf::RenderWindow &window, string data){
    float y;
    char prev = 'L';

    for(int i=0 ; i<data.length() ; i++){

        if(data[i]=='1'){
            if(prev=='L'){
                y = HIGH_Y;
                prev = 'H';
            }
            else{
                y = LOW_Y;
                prev = 'L';
            }
        }
        else y = AXIS_Y;

        drawHorizontalLine(window, START_X + i * BIT_WIDTH, START_X + (i + 1) * BIT_WIDTH, y);

        if(i < data.length()-1){
            float currentY;
            float nextY;

            currentY = y;

            char nextPrev = prev;

            if(data[i+1]=='1'){
                if(nextPrev=='L') nextY = HIGH_Y;
                else nextY = LOW_Y;
            }
            else nextY = AXIS_Y;

            if(currentY != nextY) drawTransition(window,START_X + (i + 1) * BIT_WIDTH,currentY,nextY);
        }
    }
}

void PseudoTernary(sf::RenderWindow &window, string data){
    float y;
    char prev = 'L';

    for(int i=0 ; i<data.length() ; i++){

        if(data[i]=='0'){
            if(prev=='L'){
                y = HIGH_Y;
                prev = 'H';
            }
            else{
                y = LOW_Y;
                prev = 'L';
            }
        }
        else y = AXIS_Y;

        drawHorizontalLine(window, START_X + i * BIT_WIDTH, START_X + (i + 1) * BIT_WIDTH, y);

        if(i < data.length()-1){
            float currentY;
            float nextY;

            currentY = y;

            char nextPrev = prev;

            if(data[i+1]=='0'){
                if(nextPrev=='L') nextY = HIGH_Y;
                else nextY = LOW_Y;
            }
            else nextY = AXIS_Y;

            if(currentY != nextY) drawTransition(window,START_X + (i + 1) * BIT_WIDTH,currentY,nextY);
        }
    }
}

int main()
{
    int choice;
    string data;
    int n;

    cout << endl << endl;
    cout << "1. Unipolar NRZ" << endl;
    cout << "2. Polar NRZ" << endl;
    cout << "3. NRZ inverted" << endl;
    cout << "4. Bipolar encoding" << endl;
    cout << "5. Manchester encoding" << endl;
    cout << "6. Differential Manchester encoding" << endl;
    cout << "7. AMI" << endl;
    cout << "8. PseudoTernary" << endl;
    cout << "9. EXIT" << endl;
    cout << "enter your choice : ";
    cin >> choice;

    cout << "enter the data : ";
    cin >> data;
    cout << endl;

    n = data.length();

    if(choice==9){
        cout << "EXIT" << endl;
        return 0;
    }

    sf::RenderWindow window(
        sf::VideoMode({1000,600}),
        "Line Encoding"
    );

    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>())  window.close();
        }

        window.clear(sf::Color::White);

        drawAxis(window);
        drawGrid(window, n);

        switch(choice){
            case 1:
                Unipolar_NRZ(window, data);
                break;

            case 2:
                Polar_NRZ(window, data);
                break;

            case 3:
                NRZ_Inverted(window, data);
                break;

            case 4:
                Bipolar_Encoding(window, data);  
                break;

            case 5:
                Manchester_Encoding(window, data);
                break;

            case 6:
                Differential_Manchester_Encoding(window, data);
                break;

            case 7: 
                AMI(window, data);
                break;

            case 8:
                PseudoTernary(window, data);
                break;

            default:
                break;
        }

        window.display();
    }

    return 0;
}