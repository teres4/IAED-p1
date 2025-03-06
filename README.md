-Simple Project in C made for Introduction to Algorithms and Data Structures
-This project manages public transport routes, where each route consists of a sequence of connections between stops.
Features:

    Routes: Identified by a name (max 20 bytes), with a start and end stop.
    Stops: Defined by a name (max 50 bytes) and GPS coordinates (latitude, longitude).
    Connections: Link two stops within a route, with a cost and duration (floating-point, displayed with two decimal places).
    Circular Routes: Supported when the last stop connects back to the first stop.

Limits:

    Max 200 routes, 10,000 stops, and 30,000 connections.
