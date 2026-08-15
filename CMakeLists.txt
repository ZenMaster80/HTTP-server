# ============================================================================
# MODERN C++ CMAKE TEMPLATE (Kopioitava pohja)
# ============================================================================

# 1. CMAKE-VERSIOVAATIMUS
# Määritetään alin CMake-versio, jota projektisi vaatii. 
# Versio 3.15+ on erinomainen standardi nykyisille kääntäjille.
cmake_minimum_required(VERSION 3.15)

# 2. PROJEKTIN TIEDOT
# project(ProjektinNimi KIELI_MÄÄRITYS)
# CXX tarkoittaa, että kyseessä on C++ -projekti.
project(HTTP-server VERSION 1.0.0 LANGUAGES CXX)

# ============================================================================
# C++ STANDARDIN JA KÄÄNTÄJÄN ASETUKSET
# ============================================================================

# Pakotetaan kääntäjä käyttämään tiettyä C++-standardia (esim. 17, 20 tai 23)
set(CMAKE_CXX_STANDARD 20)

# Vaaditaan, että kääntäjän ON tuettava valittua standardia. Jos ei tue, kääntäminen epäonnistuu heti.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Estetään kääntäjäkohtaiset laajennukset (kuten GNU-laajennukset), jotta koodi pysyy standardina
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# PROJEKTIN LÄHDEKOODITIEDOSTOT
# ============================================================================

# Määritetään kooditiedostot, joista ohjelma rakennetaan.
# Voit lisätä tähän listaan uusia .cpp-tiedostoja sitä mukaa kun projekti kasvaa.
set(SOURCE_FILES
    main.cpp
    HTTP-server.cpp
    HTTP-server.h
    # server.cpp      # <--- Esimerkki: poista kommentti kun lisäät uusia tiedostoja
    # utils.cpp
)

# ============================================================================
# MAALIEN (TARGETS) LUONTI
# ============================================================================

# Luodaan suoritettava konsoliohjelma (Executable).
# Ensimmäinen parametri "oma_ohjelma" on valmiin .exe/.out tiedoston nimi.
# Toinen parametri on ylempänä määritelty lista kooditiedostoista.
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# ============================================================================
# KÄÄNTÄJÄVAROITUKSET (Ammattilaisen valinta)
# ============================================================================

# Seuraavat asetukset pakottavat kääntäjän varoittamaan huonosta koodista.
# PRIVATE-avainsana tarkoittaa, että asetukset koskevat vain tätä meidän ohjelmaamme.
if(MSVC)
    # Windowsin Visual Studio kääntäjän (MSVC) varoitustasot
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    # Linuxin ja macOS:n kääntäjien (GCC / Clang) varoitustasot
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# ============================================================================
# ULKOISET KIRJASTOT JA LINKITYKSET (Käytä tarvittaessa)
# ============================================================================

# ALUSTAKOHTAINEN LINKITYS (Esim. Winsock2 Windowsilla)
if(WIN32)
    # Jos ollaan Windowsilla, linkitetään järjestelmän ws2_32 -kirjasto
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

# ----------------------------------------------------------------------------
# ESIMERKKI: Ulkoisen kirjaston (kuten Boost) etsiminen ja linkitys.
# Ota käyttöön poistamalla kommenttimerkit (#) alta, kun tarvitset Boostia.
# ----------------------------------------------------------------------------
# find_package(Boost REQUIRED COMPONENTS system)
# if(Boost_FOUND)
#     target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system)
# endif()

# ============================================================================
# INCLUDE-HAKEMISTOT (Mistä kääntäjä etsii .h/.hpp otsikkotiedostoja)
# ============================================================================

# Kertoo kääntäjälle, että projektin omia .h-tiedostoja voi etsiä samasta kansiosta
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

