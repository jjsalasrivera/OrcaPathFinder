# OrcaPathFinder

This is a simple program trying to find the shortest path between two points crosing line segments. This program is a possible solution of the [Challenge - shortest path](https://github.com/orca-io/orca-challenges/tree/master/challenge-shortest-path) promoved by [Orca](https://getorca.com/)

## Algorithm

The algorithm try to find the most optimistic path, that is, connect two points with a straight line and check if it crosses all the line segment (gateways). If does not, then reclaculate moving the end point to the next segment and repeat the process.

