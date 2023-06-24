#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>

using namespace std;

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const sf::Color BACKGROUND_COLOR = sf::Color::White;
const sf::Color VERTEX_COLOR = sf::Color::Red;
const sf::Color TEXT_COLOR = sf::Color::Black;
const sf::Color EDGE_COLOR = sf::Color::Black;

template<typename T>
class Edge;

template<typename T>
class Vertex {
public:
    T data;
    std::vector<Edge<T>*> edges;

    Vertex(T data) : data(data) {}
};

template<typename T>
class Edge {
public:
    Vertex<T>* from;
    Vertex<T>* to;
    int weight;

    Edge(Vertex<T>* from, Vertex<T>* to, int weight)
            : from(from), to(to), weight(weight) {}
};

template<typename T>
class Graph {
public:
    std::vector<Vertex<T>*> vertices;

    void addVertex(T data) {
        vertices.push_back(new Vertex<T>(data));
    }

    void addEdge(T fromData, T toData, int weight) {
        Vertex<T>* fromVertex = findVertex(fromData);
        Vertex<T>* toVertex = findVertex(toData);

        if (fromVertex && toVertex) {
            fromVertex->edges.push_back(new Edge<T>(fromVertex, toVertex, weight));
        }
    }

    Vertex<T>* findVertex(T data) {
        for (Vertex<T>* vertex : vertices) {
            if (vertex->data == data) {
                return vertex;
            }
        }
        return nullptr;
    }
};

class GraphDrawer {
private:
    Graph<string> graph;
    sf::RenderWindow window;
    sf::Font font;

public:
    GraphDrawer() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Graph Drawer") {
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Failed to load font." << std::endl;
        }
    }
    void run() {
        window.clear(BACKGROUND_COLOR);
        initializeGraph();

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            int opc = 0;
            cout << "OPCIONES:" << endl;
            cout << "1. visualizar el grafo" << endl;
            cout << "2. agregar un nuevo nodo y arista" << endl;
            cout << "3. agregar una arista" <<endl;
            cout << "4. cerrar" << endl;
            cin >> opc;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                if (opc == 1) {
                    window.clear(BACKGROUND_COLOR);
                    drawGraph();
                    window.display();
                } else if (opc == 2) {
                    string vertice;
                    cout << "Ingrese el nombre del nuevo vértice: " << endl;
                    cin >> vertice;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    } else {
                        graph.addVertex(vertice);

                        string opcion;
                        cout << "¿Desea agregar una nueva arista? (S/N): " << endl;
                        cin >> opcion;

                        if (opcion == "S" || opcion == "s") {
                            string verticeOrigen, verticeDestino;
                            int peso;

                            cout << "Ingrese el nombre del vértice de origen: " << endl;
                            cin >> verticeOrigen;
                            cout << "Ingrese el nombre del vértice de destino: " << endl;
                            cin >> verticeDestino;
                            cout << "Ingrese el peso de la arista: " << endl;
                            cin >> peso;

                            if (!cin.fail()) {
                                graph.addEdge(verticeOrigen, verticeDestino, peso);
                            } else {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                        }

                        window.clear(BACKGROUND_COLOR);
                        drawGraph();
                        window.display();
                    }
                }else if(opc == 3){
                    string verticeOrigen, verticeDestino;
                    int peso;
                    cout << "Ingrese el nombre del vértice de origen: " << endl;
                    cin >> verticeOrigen;
                    cout << "Ingrese el nombre del vértice de destino: " << endl;
                    cin >> verticeDestino;
                    cout << "Ingrese el peso de la arista: " << endl;
                    cin >> peso;

                    if (!cin.fail()) {
                        graph.addEdge(verticeOrigen, verticeDestino, peso);
                    } else {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    window.clear(BACKGROUND_COLOR);
                    drawGraph();
                    window.display();
                }else if(opc == 4){
                    window.close();
                }
            }

            sf::sleep(sf::milliseconds(100));
        }
    }





private:
    void initializeGraph() {
        graph.addVertex("arroz");
        graph.addVertex("leche");
        graph.addVertex("pan");
        graph.addVertex("frijoles");
        graph.addVertex("azucar");
        graph.addVertex("aceite");
        graph.addVertex("sal");
        graph.addVertex("harina");

        graph.addEdge("arroz", "leche", 6);
        graph.addEdge("arroz", "pan", 2);
        graph.addEdge("leche", "pan", 12);
        graph.addEdge("frijoles", "azucar", 6);
        graph.addEdge("sal", "harina", 8);
        graph.addEdge("harina", "leche", 3);
        graph.addEdge("sal", "leche", 1);
        graph.addEdge("sal", "pan", 1);
        graph.addEdge("harina", "pan", 3);
        graph.addEdge("frijoles", "sal", 1);
        graph.addEdge("azucar", "sal", 5);
        graph.addEdge("arroz", "frijoles", 9);
        graph.addEdge("aceite", "sal", 7);
        graph.addEdge("aceite", "harina", 4);
        graph.addEdge("azucar", "harina", 3);
    }

    void drawGraph() {
        for (int i = 0; i < graph.vertices.size(); i++) {
            Vertex<std::string>* vertex = graph.vertices[i];
            sf::CircleShape circle(45);
            circle.setFillColor(VERTEX_COLOR);
            circle.setPosition(getVertexPosition(i));

            sf::Text text(vertex->data, font, 20);
            text.setFillColor(BACKGROUND_COLOR);
            text.setPosition(getTextPosition(i));

            window.draw(circle);
            window.draw(text);

            for (Edge<std::string>* edge : vertex->edges) {
                Vertex<std::string>* toVertex = edge->to;
                int weight = edge->weight;

                sf::VertexArray lines(sf::Lines, 2);
                lines[0].position = getVertexCenterPosition(i);
                lines[1].position = getVertexCenterPosition(getVertexIndex(toVertex));
                lines[0].color = EDGE_COLOR;
                lines[1].color = EDGE_COLOR;

                window.draw(lines);

                sf::Text weightText(std::to_string(weight), font, 15);
                weightText.setFillColor(TEXT_COLOR);
                weightText.setPosition(getEdgeTextPosition(i, getVertexIndex(toVertex)));

                window.draw(weightText);
            }
        }
    }

    sf::Vector2f getVertexPosition(int index) {
        const float radius = 350;
        const float center_x = WINDOW_WIDTH / 2;
        const float center_y = (WINDOW_HEIGHT / 2) - 150;

        float angle = index * (2 * M_PI) / graph.vertices.size();
        float x = center_x + radius * cos(angle);
        float y = center_y + radius * sin(angle);

        return sf::Vector2f(x, y);
    }

    sf::Vector2f getVertexCenterPosition(int index) {
        sf::Vector2f vertexPosition = getVertexPosition(index);
        const float radius = 30;
        float x = vertexPosition.x + radius;
        float y = vertexPosition.y + radius;
        return sf::Vector2f(x, y);
    }

    sf::Vector2f getTextPosition(int index) {
        sf::Vector2f vertexPosition = getVertexPosition(index);
        const float textOffsetX = 25;
        const float texoffsetY = 25;
        float x = vertexPosition.x + textOffsetX;
        float y = vertexPosition.y + texoffsetY;
        return sf::Vector2f(x, y);
    }

    sf::Vector2f getEdgeTextPosition(int fromIndex, int toIndex) {
        sf::Vector2f fromPosition = getVertexPosition(fromIndex);
        sf::Vector2f toPosition = getVertexPosition(toIndex);
        const float textOffsetX = 30;
        const float textOffsetY = -30;
        float x = (fromPosition.x + toPosition.x) / 2 + textOffsetX;
        float y = (fromPosition.y + toPosition.y) / 2 + textOffsetY;
        return sf::Vector2f(x, y);
    }

    int getVertexIndex(Vertex<std::string>* vertex) {
        for (int i = 0; i < graph.vertices.size(); i++) {
            if (graph.vertices[i] == vertex)
                return i;
        }
        return -1;
    }

};

int main() {
    GraphDrawer graphDrawer;
    graphDrawer.run();

    return 0;
}
