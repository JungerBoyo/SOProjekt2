add_executable(${PROJECT_NAME}
  main.c
  Command.c
  Global.c

  WritersStarvation.c
  ReadersStarvation.c
  NoStarvation.c
)

target_link_libraries(${PROJECT_NAME} pthread)