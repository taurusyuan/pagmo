 /*****************************************************************************
 *   Copyright (C) 2004-2013 The PaGMO development team,                     *
 *   Advanced Concepts Team (ACT), European Space Agency (ESA)               *
 *   http://apps.sourceforge.net/mediawiki/pagmo                             *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Developers  *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Credits     *
 *   act@esa.int                                                             *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *****************************************************************************/
#include <iostream>
#include <iomanip>

#include "../src/pagmo.h"

/*
 * This test file creates a random vector<vector<double>> two dimensional vector,
 * then it instantiates a tsp object which loads the vector object.
 * It then internally converts the vector to a boost graph,
 * which is then returned and checked against the initial 2D vector.
 */

using namespace pagmo;

problem::vector2D<double> generate_random_vector2D(int dimension, bool verbose = false) {
    std::default_random_engine rengine(time(NULL)); // seed software PRNG
    std::uniform_real_distribution<> distr(0, 1); // range
    
    problem::vector2D<double> random_2D(dimension, std::vector<double>(dimension, 0));

    if (verbose) std::cout << "Two dimensional matrix created:";
    
    for (int i = 0; i < dimension; ++i) {
        if (verbose) std::cout << std::endl;
        
        for (int j = 0; j < dimension; ++j) {
            if (i == j) 
                random_2D[i][j] = 0;
            else
                random_2D[i][j] = distr(rengine);

            if (verbose) std::cout << random_2D[i][j] << " \t "; 
        }
    }
    if (verbose) std::cout << std::endl ;
    
    return random_2D;
}

int main()
{
    // create random 2d vector and output it to console
    int no_vertices = rand() % 10 + 1; // between 1 and 10
    problem::vector2D<double> original( generate_random_vector2D(no_vertices, true) );
    
    // instantiate a tsp problem, vector constructor is called
    pagmo::problem::tsp tsprob(original);
    
    // output the graph structure, conversion done internally
    std::cout << tsprob.human_readable();
    
    // get the converted graph
    problem::tsp_graph graph = tsprob.get_graph();
    
    // convert back to vector2D
    problem::vector2D<double> converted(no_vertices, std::vector<double>(no_vertices, 0));
    pagmo::problem::base_tsp::convert_graph_to_vector2D(graph, converted);
    
    // check equality
    if (original != converted) {
        std::cout << "vector2D to boost graph to vector2D conversion failed!\n";
        return 1;
    }
    
    // some testing stuff
    
    int n = 4; int index; int k = 0;
    problem::vector2D<int> matt(n, std::vector<int>(n, 0));
    std::vector<int> v(n*(n-1), 0);
    std::vector<int> c(n*2-1, 0);
    
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            matt[i][j] = i*n + j;
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(i==j) { ++k; continue; }

            index = i*n + j - k;
            std::cout << i << " - " << j << " = " << matt[i][j] << " \t idx: " << index << std::endl;
            v[index] = matt[i][j];
        }
    }
    
    for (int i = 0; i < n*(n-1) ; ++i){
        std::cout << v[i] << " , ";
    }
    std::cout << std::endl << std::endl;
    
    k = 0;
    
    int ics = 0; k = 0;
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < v.size(); j += (n-1) ) {
            c[i+n] += v[i+j];


            ics++;
            std::cout << j << " - " << i << " -- " << j/(n-1) << " - " << v[j / (n-1) + i * n] << " k = " << k << std::endl;


            c[k] += v[j / (n-1) + i * n]; 
            if ( ics % (n-1) == 0) ++k;
        }
    }
    
    for (int i = 0; i < c.size(); ++i)
        std::cout << c[i] << ", ";
    
    return 0;
}