add_subdirectory(dataclasses)

target_sources(Bullhorn PRIVATE
    main.cpp
    Driver.cpp
    TimeParser.cpp
    FileSysOp.cpp
    AudioPlayer.cpp
)
