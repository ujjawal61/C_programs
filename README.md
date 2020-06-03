# C_programs

## igncsv.c
This program sorts out the games according to their rating . Data was provided  by ign containg games from last 20 years and quicksorting is applied 

## dijkstrac.c
Dublin Bus now provides real-time updates on the location and expected time of arrival for
their buses. Google Maps use this information to advise you on the best sequence of
buses/trains to take in order to reach your desired destination in the shortest possible time.
This is achieved by viewing Dublin as being comprised of a number of nodes in a graph
(locations) and edges between those nodes (roads/bus routes/train tracks). Each edge has a
weight which depends on how long it will take you to travel towards your intended
destination via that route. 
Given nodes, edges and weights, Dijkstra’s algorithm can be used to determine the optimal route to get you from where you are to where you want to be.
There are two files – one contains a list of all bus
stops in Dublin (nodes) and the other contains a list of routes between those bus stops
(edges). The weights on each of the edges is the distance in metres between each bus stop.
(This is real data which is publicly available via a live API. You can grab more complete
information from here if you are interested on extending this problem:
https://data.dublinked.ie/ )
Program is building a graph which models the
public transport system of Dublin city. It take as input a starting bus stop
ID and a destination bus stop ID. Using Dijkstra’s algorithm on the graph, print the optimal
sequence of bus stops from source to destination. 
