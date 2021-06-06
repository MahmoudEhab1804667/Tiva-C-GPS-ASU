#include <stdio.h> 
#include"math.h"
double calculate_Distance(double latHome, double lonHome, double latDest, double lonDest) {
    double pi = 3.141592653589793;
    double Radius = 6371.0;
    double latHome2 = (pi / 180.0) * (latHome);
    double latDest2 = (pi / 180.0) * (latDest);
    double difference_Lat = (pi / 180.0) * (latDest - latHome);
    double difference_Lon = (pi / 180.0) * (lonDest - lonHome);
    double a = sin(difference_Lat / 2.0) * sin(difference_Lat / 2.0) + cos(latHome2) * cos(latDest2) * sin(difference_Lon / 2.0) * sin(difference_Lon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    double distance = Radius * c;
    printf("%f\n", distance);
    return distance;
}