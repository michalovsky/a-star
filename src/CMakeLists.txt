set(SOURCES
        UserInputReader.cpp
        RectangleSfml.cpp
        LineSfml.cpp
        GraphicsIdGenerator.cpp
        MapBuilder.cpp
        MapRenderer.cpp
        PathFinding.cpp
        AStarApp.cpp
        AStarAppFactory.cpp
        Main.cpp
        )

add_executable(a-star ${SOURCES})
target_link_libraries(a-star PRIVATE ${SFML_LIBRARIES})
target_include_directories(a-star PUBLIC src)