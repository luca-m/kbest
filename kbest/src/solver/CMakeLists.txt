
add_executable(kbest_cli KBest_cli.cpp  
                          KBest.cpp  KBest.h  
                          Matrix.cpp  Matrix.h  
                          Problem.cpp  Problem.h  
                          Solution.cpp  Solution.h)
INSTALL(TARGETS kbest_cli DESTINATION ${BIN_DIR})

ADD_LIBRARY(kbest SHARED KBest.cpp Matrix.cpp Problem.cpp Solution.cpp)
INSTALL(TARGETS kbest DESTINATION ${BIN_DIR})
