// graph.h <Starter Code>
// < Your name >
//
// Basic graph class using adjacency matrix representation.  Currently
// limited to a graph with at most 100 vertices.
//
// University of Illinois at Chicago
// CS 251: Fall 2020
// Project #7 - Openstreet Maps

#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>

using namespace std;

template<typename VertexT, typename WeightT>
class graph {
 private:
  int count_edge;
  map <VertexT, map <VertexT, WeightT> > adjList;

 public:
  graph() {
    count_edge = 0;
  }
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const {
     return adjList.size();
  }

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const {
    return count_edge;
  }
  // addVertex
  // Adds the vertex v to the graph if there's room, and if so
  // returns true.  If the graph is full, or the vertex already
  // exists in the graph, then false is returned.
  bool addVertex(VertexT v) {
    // create temp adjList map
    map <VertexT, WeightT> tempAdjListMapt;
    // see if there are contents in map;
    if (adjList.count(v) > 0) {
        return false;
    }
    // add vertex;
    adjList[v] = tempAdjListMapt;
    return true;
  }

  //
  // addEdge
  //
  // Adds the edge (from, to, weight) to the graph, and returns
  // true.  If the vertices do not exist or for some reason the
  // graph is full, false is returned.
  //
  // NOTE: if the edge already exists, the existing edge weight
  // is overwritten with the new edge weight.
  //
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    // see if map isnt empty
    if (adjList.count(from) > 0 && adjList.count(to) > 0) {
        // see if there are duplicates.
        if (adjList[from].count(to) == 0) {
            // overwrite duplicates and add them to map
                adjList[from].emplace(to,weight);
                // increment the edge counter
                count_edge++;
            } else{
                adjList[from][to] = weight;
            }
            return true;
    } else {
        return false;
    }
  }

  // getWeight
  //
  // Returns the weight associated with a given edge.  If
  // the edge exists, the weight is returned via the reference
  // parameter and true is returned.  If the edge does not
  // exist, the weight parameter is unchanged and false is
  // returned.
  //
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
    if (adjList.count(from) > 0 && adjList.count(to) > 0) {
        if (adjList.at(from).find(to) == adjList.at(from).end()) {
           return false;
        } else {
          weight = adjList.at(from).at(to);
            return true;
        }
    } else {
     return false;
    }
  }
  // neighbors
  // Returns a set containing the neighbors of v, i.e. all
  // vertices that can be reached from v along one edge.
  // Since a set is returned, the neighbors are returned in
  // sorted order; use foreach to iterate through the set.
  set<VertexT> neighbors(VertexT v) const {
    // make temporary set 
    set<VertexT>  S;
    if (adjList.count(v) > 0) {
        // loop for the contents inside the adjacency list vertex
       for (auto i : adjList.at(v)) {
           // insert into set
           S.insert(i.first);
       }
    } else {
        return S;
    }
    return S;
  }

  //
  // getVertices
  //
  // Returns a vector containing all the vertices currently in
  // the graph.
  //

  vector<VertexT> getVertices() const {
    vector <VertexT> copy;
    // loop and see/ add copys 
    for (auto i : adjList) {
        copy.push_back(i.first);
    }
    return  copy;  // returns a copy:
  }


  // dump
  //
  // Dumps the internal state of the graph for debugging purposes.
  // Example:
  //    graph<string,int>  G(26);
  //    ...
  //    G.dump(cout);  // dump to console
  void dump(ostream& output) const {
    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices: " << this->NumVertices() << endl;
    output << "**Num edges: " << this->NumEdges() << endl;

    output << endl;
    output << "**Vertices:" << endl;
    int x = 0;
    for (auto i : adjList) {
        output << " "<< x <<". " << i.first << endl;
        x++;
    }
    output << endl;
    output << "**Edges:" << endl;

    for (auto i : adjList) {
        if (i.second.size() == 0) {
          continue;
        }
        output << i.first << ": ";
        for (auto g : i.second) {
            output << "(" << i.first<< "," << g.first << "," << g.second << ")";
        }
        cout << endl;
    }
    output << endl;
    output << "**************************************************" << endl;
  }
};
