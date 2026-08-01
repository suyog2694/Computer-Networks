#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;

void Unipolar_NRZ(string data, int n){
    cout << "Unipolar NRZ encoding for data : " << data << endl ;
    cout << "1-above(*)   0-on axis(-)" << endl ;

    for (int i=0 ; i<n ; i++){
        if(data[i]=='1'){
            cout << "*" ;
        }
        else if(data[i]=='0'){
            cout << "-" ;
        }
        else{
            cout << "error" ;
            break ;
        }
    }
}

void Polar_NRZ(string data, int n){
    cout << "Polar NRZ encoding for data : " << data << endl ;
    cout << "1-below(_)   0-above(*)" << endl ;

    for (int i=0 ; i<n ; i++){
        if(data[i]=='1'){
            cout << "_" ;
        }
        else if(data[i]=='0'){
            cout << "*" ;
        }
        else{
            cout << "error" ;
            break ;
        }
    }
}

void NRZ_inverted(string data, int n){
    cout << "NRZ inverted encoding for data : " << data << endl ;
    cout << "1-transition   0-No transition" << endl ;

    char prev = '*' ;

    for (int i=0 ; i<n ; i++){
        if (i==0){
            cout << prev ;
        }
        else if(data[i]=='1'){
            if(prev=='*'){
                prev = '_' ;
                cout << prev ;
            }
            else{
                prev = '*' ;
                cout << prev ;
            }       
        }
        else if(data[i]=='0'){
            cout << prev ;
        }
        else{
            cout << "error" ;
            break ;
        }
    }
}

void Bipolar_Encoding(string data, int n){
    cout << "Bipolar encoding for data : " << data << endl ;
    cout << "1- ^ & s alternately      0-on axis(-)" << endl ;

    char prev = 's' ;
    
    for (int i=0 ; i<n ; i++){
        if(data[i]=='1'){
            if(prev=='s'){
                prev = '^' ;
                cout << prev ;
            }
            else{
                prev = 's' ;
                cout << prev ;
            }
        }
        else if(data[i]=='0'){
            cout << "-" ;
        }
        else{
            cout << "error" ;
            break ;
        }
    }
}

void Manchester_Encoding(string data, int n){
    cout << "Manchester encoding for data : " << data << endl ;
    cout << "1- below to above (S)     0-above to below (Z)" << endl ;

    for (int i=0 ; i<n ; i++){
        if(data[i]=='1'){
            cout << "S " ;
        }
        else if(data[i]=='0'){
            cout << "Z " ;
        }
        else{
            cout << "error" ;
            break ;
        }
    }
}

void Differential_Manchester_Encoding(string data, int n){
    cout << "Differential Manchester encoding for data : " << data << endl ;
    cout << "1- transition in pattern(S->Z || Z->S)     0-no transition in pattern (S->S || Z->Z)" << endl ;

    char prev = 'Z' ;
    
    for (int i=0 ; i<n ; i++){
        if(data[i]=='0'){
            cout << prev ;
        }
        else{
            if(prev=='Z'){
                prev = 'S' ;
                cout << prev ;
            }
            else{
                prev = 'Z' ;
                cout << prev ;
            }
        }
    }
}

void AMI(string data, int n){
    cout << "AMI encoding for data : " << data << endl ;
    cout << "1- above(*) & below(_) alternately      0-on axis(-)" << endl ;

    char prev = '_' ;

    for (int i=0 ; i<n ; i++){
        if(data[i]=='0') cout << '-' ;
        else if(data[i]=='1' && prev=='_'){
            prev = '*' ;
            cout << prev ;
        }
        else {
            prev = '_' ;
            cout << prev ;
        }
    }
}

void PseudoTernary(string data, int n){
    cout << "PseudoTernary encoding for data : " << data << endl ;
    cout << "0- above(*) & below(_) alternately      1-on axis(-)" << endl ;

    char prev = '_' ;

    for (int i=0 ; i<n ; i++){
        if(data[i]=='1') cout << '-' ;
        else if(data[i]=='0' && prev=='_'){
            prev = '*' ;
            cout << prev ;
        }
        else {
            prev = '_' ;
            cout << prev ;
        }
    }
}

int main(){
    int choice ;
    string data ;
    int n ;
    
    cout << endl << endl ;
    cout << "1. Unipolar NRZ" << endl ;
    cout << "2. Polar NRZ" << endl ;
    cout << "3. NRZ inverted" << endl ;
    cout << "4. Bipolar encoding" << endl ;
    cout << "5. Manchester encoding" << endl ;
    cout << "6. Differential Manchester encoding" << endl ;
    cout << "7. AMI" << endl ;
    cout << "8. PseudoTernary" << endl ;
    cout << "9. EXIT" << endl ;
    cout << "enter your choice : " ;
    cin >> choice ;

    cout << "enter the data : " ;
    cin >> data ;
    cout << endl ;
    n = data.length() ;

    if (choice==9){
        cout << "EXIT" << endl ;
        return 0 ;
    }

    sf::RenderWindow window(
        sf::VideoMode({1000, 600}),
        "Line Encoding");

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);

        // Horizontal axis
        sf::Vertex axis[2];

        axis[0].position = {50.f, 300.f};
        axis[0].color = sf::Color::Black;

        axis[1].position = {950.f, 300.f};
        axis[1].color = sf::Color::Black;

        window.draw(axis, 2, sf::PrimitiveType::Lines);

        float startX = 50.f;
        float bitWidth = 80.f;

        for (int i = 0; i <= n; i++)
        {
            sf::Vertex line[2];

            line[0].position = {startX + i * bitWidth, 250.f};
            line[0].color = sf::Color(180, 180, 180);

            line[1].position = {startX + i * bitWidth, 350.f};
            line[1].color = sf::Color(180, 180, 180);

            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        window.display();
    }

    return 0;
}