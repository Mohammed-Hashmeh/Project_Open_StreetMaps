// application.cpp <Starter Code>
// <Your name>
//
// University of Illinois at Chicago
// CS 251: Fall 2020
// Project #7 - Openstreet Maps
//
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <set>
#include <queue>
#include <stack>
#include "limits"
#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"
#include "graph.h"

using namespace std;
using namespace tinyxml2;

class prioritize
{
public:
      bool operator()(const pair<long long, double>& p1,
      const pair<long long, double>& p2) const {
        // if you want smaller values @ the end, return true if
        // p1's value < p2's value.  if you want larger values
        // @ the end, return true if p1's value > p2's value.
        if (p1.second > p2.second)
          return true;
        else if (p1.second < p2.second)
          return false;
        else  // values are the same, decide based on keys:
          return p1.first > p2.first;
      }
};

// Dijkstra's algorirtim to find the shortest path, takes in a range
// of different data structures in order to get the path
// and the distance, return is a vector, long long visited
vector<long long> Dijk(graph<long long, double>& G,
  long long startV, map<long long, double>& distances,
  map <long long, long long>& predV) {
  priority_queue<pair<long long, double>,
  vector<pair<long long, double>>, prioritize> unvisitedQueue;
    vector<long long>  visited;
    vector<long long> vertices = G.getVertices();
    double INF = numeric_limits<double>::max();
    set<long long> visitedSet;
    double alternativePathDistance = 0;
    double edgeWeight = 0;

    for (auto currentV : vertices) {
        unvisitedQueue.push(make_pair(currentV, INF));
        distances[currentV] = INF;
        predV[currentV] = -1;
    }
    // startV has a distance of 0 from itself
    distances[startV] = 0;
    unvisitedQueue.push(make_pair(startV, 0));

    while (!unvisitedQueue.empty()) {
    // Visit vertex with minimum distance from startV
        pair<long long, double> currV;
        currV = unvisitedQueue.top();
        unvisitedQueue.pop();

        if (distances[currV.first] == INF) {
            break;
        } else if (visitedSet.count(currV.first) > 0) {
            continue;
        } else {
            visitedSet.insert(currV.first);
            visited.push_back(currV.first);
        }

        set<long long> adjcent = G.neighbors(currV.first);

        for (auto adjV : adjcent) {
            edgeWeight = 0;
            G.getWeight(currV.first, adjV, edgeWeight);
            alternativePathDistance = distances[currV.first] + edgeWeight;
            // If shorter path from startV to adjV is found,
            // update adjV's distance and predecessor
            if (alternativePathDistance < distances[adjV]) {
                distances[adjV]= alternativePathDistance;
                predV[adjV] = currV.first;
                unvisitedQueue.push(make_pair(adjV, alternativePathDistance));
            }
        }
    }
    return visited;
}

// function prints the shortest path, uses Dijkstras alogrithm to get
// the predecessor and push into a stack inorder to print the path backwards
// function takes in many data structures as parameters, prints to console
void printPath(double Nearest_Dest, double Nearest_Start,
bool reachable_destination, map <long long, long long> predecessor,
    stack <long long> print_pred, map<long long,Coordinates> Nodes,
    map<long long, double> distances) {
    long long pred;
    // push the shortest path into the stack

    print_pred.push(Nodes[Nearest_Dest].ID);
    pred = Nodes[Nearest_Dest].ID;
    // serious of checkstatements to see if the path exist
    for (int i = 0; i< predecessor.size(); i++) {
        if (predecessor[pred] == -1 && pred == Nodes[Nearest_Start].ID) {
            reachable_destination = true;
            break;
        }
      if (predecessor[pred] == -1 && pred != Nodes[Nearest_Start].ID) {
            break;
        }
        pred = predecessor[pred];
        print_pred.push(pred);
    }
    if (reachable_destination) {
        cout << "Distance to dest: " << distances[Nearest_Dest]
        << " miles" << endl;
        cout << "Path: ";
    while (!print_pred.empty()) {
        if (print_pred.size() -1 == 0) {
            cout << print_pred.top();
        } else {
        cout << print_pred.top() << "->";
        }
        print_pred.pop();
      }
    } else {
        cout << "Sorry, destination unreachable";
    }
    cout << endl;
    cout << endl;
}
// function takes in vector buildinginfo, and building name
// function finds the abbreviation or the full name and see s if it exist
// in the vector, returns position
int Find_Building(vector <BuildingInfo> B, string B_Name) {
    int pos = -1;
    for (int i = 0; i < B.size(); i++) {
        if (B[i].Abbrev == B_Name) {
            return i;
        }
        pos = B[i].Fullname.find(B_Name);
        if (pos != -1) {
            return i;
        }
    }
    return -1;
}
// finds the nearest footway, takes in map<long long,Coordinates> Nodes,
// vector<FootwayInfo> Footways, double lat, double lon, returns minium Footways
long long int Find_NearestFootWay(map<long long, Coordinates> Nodes,
    vector<FootwayInfo> Footways, double lat, double lon) {
    double min = distBetween2Points(Nodes[Footways[0].Nodes[0]].Lat,
    Nodes[Footways[0].Nodes[0]].Lon, lat, lon);
    long long int index = Footways[0].Nodes[0];
    double temp;
    for (int i = 0; i < Footways.size(); i++) {
        int size = Footways[i].Nodes.size();
        for (int j = 0; j < size; j++) {
            temp = distBetween2Points(Nodes[Footways[i].Nodes[j]].Lat,
            Nodes[Footways[i].Nodes[j]].Lon, lat, lon);
            if (temp < min) {
                 index = Footways[i].Nodes[j];
                 min = temp;
             }
        }
    }
    return index;
}


int main() {
  // maps a Node ID to it's coordinates (lat, lon)
  map<long long, Coordinates>  Nodes;
  // info about each footway, in no particular order
  vector<FootwayInfo>          Footways;

  graph<long long, double>  G;  // vertices are nodes, weights are distances

  // info about each building, in no particular order
  vector<BuildingInfo>         Buildings;
  XMLDocument                  xmldoc;
  cout << "** Navigating UIC open street map **" << endl;
  cout << endl;
  cout << std::setprecision(8);

  string def_filename = "map.osm";
  string filename;

  cout << "Enter map filename> ";
  getline(cin, filename);

  if (filename == "") {
    filename = def_filename;
  }

  //
  // Load XML-based map file
  if (!LoadOpenStreetMap(filename, xmldoc)) {
    cout << "**Error: unable to load open street map." << endl;
    cout << endl;
    return 0;
  }

  //
  // Read the nodes, which are the various known positions on the map:
  //
  int nodeCount = ReadMapNodes(xmldoc, Nodes);

  //
  // Read the footways, which are the walking paths:
  //
  int footwayCount = ReadFootways(xmldoc, Footways);

  // Read the university buildings:
  //
  int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);
    assert(nodeCount == (int)Nodes.size());
    assert(footwayCount == (int)Footways.size());
    assert(buildingCount == (int)Buildings.size());

    cout << endl;
    cout << "# of nodes: " << Nodes.size() << endl;
    cout << "# of footways: " << Footways.size() << endl;
    cout << "# of buildings: " << Buildings.size() << endl;
    // add vertex to graph
    for (auto i : Nodes) {
        G.addVertex(i.first);
    }
    // add edge to graph
    double dist;
    for (int i = 0; i < Footways.size(); i++) {
        int size = Footways[i].Nodes.size();
        for (int j = 0; j <size-1; j++) {
            dist = distBetween2Points(Nodes[Footways[i].Nodes[j]].Lat,
            Nodes[Footways[i].Nodes[j]].Lon, Nodes[Footways[i].Nodes[j+1]].Lat,
            Nodes[Footways[i].Nodes[j+1]].Lon);
            G.addEdge(Footways[i].Nodes[j], Footways[i].Nodes[j+1], dist);
            G.addEdge(Footways[i].Nodes[j+1], Footways[i].Nodes[j], dist);
        }
    }

    // print out edge and vertices
    cout << "# of vertices: " << G.NumVertices() << endl;
    cout << "# of edges: " << G.NumEdges() << endl;
    cout << endl;


  // Navigation from building to building

  string startBuilding, destBuilding;
  cout << "Enter start (partial name or abbreviation), or #> ";
  getline(cin, startBuilding);

  while (startBuilding != "#") {
    cout << "Enter destination (partial name or abbreviation)> ";
    int Start_BuildingVal = Find_Building(Buildings, startBuilding);
    getline(cin, destBuilding);
    int Dest_BuildVal = Find_Building(Buildings, destBuilding);

    // check if building exist
    if (Start_BuildingVal == -1) {
        cout << "Start building not found" << endl;
        cout << endl;
        cout << "Enter start (partial name or abbreviation), or #> ";
    getline(cin, startBuilding);
        continue;
    }
    if (Dest_BuildVal == -1) {
        cout << "Destination building not found" << endl;
          cout << "Enter start (partial name or abbreviation), or #> ";
         getline(cin, startBuilding);
        continue;
    }
    // print Coordinates of the building
    cout << "Starting point:" << endl;
    cout << " " << Buildings[Start_BuildingVal].Fullname << endl;
    cout << " (" << Buildings[Start_BuildingVal].Coords.Lat << ", "
    << Buildings[Start_BuildingVal].Coords.Lon << ")" << endl;


    cout << "Destination point:" << endl;
    cout << " " << Buildings[Dest_BuildVal].Fullname << endl;
    cout << " (" << Buildings[Dest_BuildVal].Coords.Lat << ", "
    << Buildings[Dest_BuildVal].Coords.Lon << ")" << endl << endl;

    double LAT = Buildings[Start_BuildingVal].Coords.Lat;
    double LON = Buildings[Start_BuildingVal].Coords.Lon;

    long long int Nearest_Start = Find_NearestFootWay(Nodes, Footways, LAT,
    LON);

    // print the nearest start and desination node
    cout << "Nearest start node:" << endl;
    cout << " " << Nodes[Nearest_Start].ID << endl;
    cout << " (" << Nodes[Nearest_Start].Lat << ", "
    << Nodes[Nearest_Start].Lon << ")" << endl;

     LAT = Buildings[Dest_BuildVal].Coords.Lat;
     LON = Buildings[Dest_BuildVal].Coords.Lon;

    long long int Nearest_Dest = Find_NearestFootWay(Nodes, Footways, LAT, LON);

    cout << "Nearest destination node:" << endl;
    cout << " " << Nodes[Nearest_Dest].ID << endl;
    cout << " (" << Nodes[Nearest_Dest].Lat << ", "
    << Nodes[Nearest_Dest].Lon << ")" << endl << endl;

    // TO DO: lookup buildings, find nearest start and dest nodes,
    // run Dijkstra's alg, output distance and path to destination:
    // initilize refrence data structers for Dijkstra algorirtim
    vector <long long> visited;
    map<long long, double> distances;
    map <long long, long long> predecessor;

    cout << "Navigating with Dijkstra..." << endl;
    visited = Dijk(G, Nearest_Start, distances, predecessor);

    stack <long long> print_pred;
    long long pred;
    bool reachable_destination = false;

    // call to print the shortest path
    printPath(Nearest_Dest, Nearest_Start,
    reachable_destination,predecessor, print_pred, Nodes, distances);
    cout << "Enter start (partial name or abbreviation), or #> ";
    getline(cin, startBuilding);
    if (startBuilding == "#") {
        break;
    }
  }
  cout << "** Done **" << endl;
  return 0;
}
