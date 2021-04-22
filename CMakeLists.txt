cmake_minimum_required(VERSION 3.17)
project(ClusterAnalyses)

set(CMAKE_CXX_STANDARD 14)

add_executable(ClusterAnalyses main.cpp Buffer.cpp Buffer.h Cloud.cpp Cloud.h Interface.cpp Interface.h Field.cpp Field.h Cluster_Search.cpp Cluster_Search.h Controller.cpp Controller.h Point.cpp Point.h Symm_Matrix.cpp Symm_Matrix.h TreeNode.h Wave_algorithm.cpp Wave_algorithm.h DBscan_algorithm.cpp DBscan_algorithm.h Kmeans_algorithm.cpp Kmeans_algorithm.h Kmeanswc_algorithm.cpp Kmeanswc_algorithm.h)