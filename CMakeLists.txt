cmake_minimum_required(VERSION 3.15)

# Määritetään koko projektin nimi
project(HTTP-server-kokonaisuus LANGUAGES CXX)

# Käsketään CMakea hyppäämään alakansioon, jossa varsinainen koodi ja toinen CMake sijaitsevat
# (Muuta "HTTP-server" vastaamaan alakansiosi tarkkaa nimeä, jos se on eri)
add_subdirectory(HTTP-server)