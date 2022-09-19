# SurfaceVoronoi: Efficiently Computing Surface Based Voronoi Diagrams Using Arbitrary Distance Solver

Created: September 18, 2022 9:24 PM
Tags: Literature Reviews

**Introduction**

- **Challenges**
    - The choice of geodesic algorithms is highly coupled with designing the scheme for computing surface based VDs 缺少普适性？
    - The known algorithms are not scalable enough to provide an all-in-one solution
    - Not easy to adapt RVD to deal with a thin-sheet model
- **Contributions**
    - Develop an over-propagation technique to work with the user-specified mark-and-sweep geodesic algorithm
    - Use **squared distance** to define the linear change in a triangle
- **Features of the proposed algorithm**
    - It can work with an arbitrary geodesic algorithm, and even supports the straight-line distance as the driving measure
    - The proposed algorithm is scalable enough to deal with various versions of surface based Voronoi diagrams

**Related Work**

- **VD in Euclidean spaces**
- **VD on 2-manifold surfaces**
    - Operating on the mesh surface directly based on geodesic distance
    - Operating on the parametrization plane
    - Operating in the 3D Euclidean space with the help of a 3D Voronoi/Delaunay solver
- **In this paper**
    - Propose an all-in-one solution to decouple the geodesic solver from the computation of surface VDs
    - Bridge the gap between geodesic VDs and Euclidean VDs

**Overview**

- Use the squared distance to define a parabolic surface rooted at each site, the lower envelope can also report the VD
- **Compute surface based VDs based on two novel techniques**
    - When one infers the lower envelope for a triangle, replace the distance value at a vertex by the squared distance
    - Computing a full distance field for every source site is time-consuming and not necessary, so the authors propose a kind of **over-propagation** distance field, where the distance field rooted at pi sweeps outward until it does not help determine the surface based VD at all

**Algorithms**

![截屏2022-09-19 11.46.03.png](SurfaceVoronoi%20Efficiently%20Computing%20Surface%20Based%20a7c87449c66d4d6cbcb4e14676aef77c/%25E6%2588%25AA%25E5%25B1%258F2022-09-19_11.46.03.png)

![截屏2022-09-19 11.46.46.png](SurfaceVoronoi%20Efficiently%20Computing%20Surface%20Based%20a7c87449c66d4d6cbcb4e14676aef77c/%25E6%2588%25AA%25E5%25B1%258F2022-09-19_11.46.46.png)

- **Over-propagation**
    - **Mark-and-sweep**
        - Computing a distance field or finding a shortest path on a polygonal surface S
        - Propagating from near to far and prioritizing near-source distance propagation events
        - Terminates when each vertex gets a distance to the nearest source point, resulting in a multi-source distance field
        - Why “such a distance field is not informative enough to accurately trace the bisectors”?
    - **Filtering rule in over propagation**
        - Keeping all the helpful source points for each triangle, rather than keeping only the nearest source for each vertex
        - A source point *p* is said to be a contributing site to the triangle *f* if *p* can provide the shortest distance for at least one point in *f*
    - **Finding the lower envelope of half-planes**
    - **Implementation**
        - 为什么要这么计算？
- **Using squared distance to define half-planes**
    - **Three indicators to characterize if a surface based VD is acceptable**
        
        ![截屏2022-09-19 11.43.53.png](SurfaceVoronoi%20Efficiently%20Computing%20Surface%20Based%20a7c87449c66d4d6cbcb4e14676aef77c/%25E6%2588%25AA%25E5%25B1%258F2022-09-19_11.43.53.png)
        
        - **Compatibility**
        - **Consistence**
            - The extracted VD does not include breakpoints at mesh edges, i.e., for each mesh edge, the entry point of the VD must be identical to the exit point
        - **Connectedness**
            - No site is allowed to dominate multiple disconnected regions - each site dominates a connected region
            - No ownerless region - all the regions form a covering of the whole surface
            - 什么时候会出现 ownerless region？
    - **Definition 1**
        
        ![截屏2022-09-19 11.37.41.png](SurfaceVoronoi%20Efficiently%20Computing%20Surface%20Based%20a7c87449c66d4d6cbcb4e14676aef77c/%25E6%2588%25AA%25E5%25B1%258F2022-09-19_11.37.41.png)
        
        - Plane
        - Height
    - **Definition 2**
        
        ![截屏2022-09-19 11.38.34.png](SurfaceVoronoi%20Efficiently%20Computing%20Surface%20Based%20a7c87449c66d4d6cbcb4e14676aef77c/%25E6%2588%25AA%25E5%25B1%258F2022-09-19_11.38.34.png)
        
        - Parabolic surface
        - Tangent plane
        - Height
    - **Lemma**
        
        ![截屏2022-09-19 11.39.26.png](SurfaceVoronoi%20Efficiently%20Computing%20Surface%20Based%20a7c87449c66d4d6cbcb4e14676aef77c/%25E6%2588%25AA%25E5%25B1%258F2022-09-19_11.39.26.png)
        
        ![截屏2022-09-19 11.39.46.png](SurfaceVoronoi%20Efficiently%20Computing%20Surface%20Based%20a7c87449c66d4d6cbcb4e14676aef77c/%25E6%2588%25AA%25E5%25B1%258F2022-09-19_11.39.46.png)
        
    - **Theorem**
        
        ![截屏2022-09-19 11.40.45.png](SurfaceVoronoi%20Efficiently%20Computing%20Surface%20Based%20a7c87449c66d4d6cbcb4e14676aef77c/%25E6%2588%25AA%25E5%25B1%258F2022-09-19_11.40.45.png)
        
    - **Remark on Theorem**
        
        ![截屏2022-09-19 11.41.21.png](SurfaceVoronoi%20Efficiently%20Computing%20Surface%20Based%20a7c87449c66d4d6cbcb4e14676aef77c/%25E6%2588%25AA%25E5%25B1%258F2022-09-19_11.41.21.png)
        
    - **Computing surface based VDs**
        - **Exact geodesic Voronoi diagram (EGVD)** - an exact geodesic algorithm
        - **Approximate geodesic Voronoi diagram (AGVD)** - the fast marching method
        - **Straight-line distance based Voronoi diagram (SLDBVD)** - straight-line distance

**Evaluation**

- **Experimental setting**
- **Geodesic distances**
    - Exact geodesic distance
    - Approximate geodesic distance
    - Commute-time distance
    - Biharmonic distance
    - Straight-line distance
- **Comparison with the state of the arts**
    - **RVD**
        - May violate the “one site, one region” property - thin parts - producing many ownerless regions
    - **LRVD**
        - The Dijkstra-sweep along mesh edges may fail to report reliable neighboring relationship between sites on poorly triangulated surfaces
    - **SLDBVD**
        - Sweep the surface from triangle to triangle while coordinating the propagation by the straight-line distance, rather than the graph based distance reported by Dijkstra’s algorithm
        - Independent of triangulation quality
    - **Both LRVD and SLDBVD have the “one site, one region” property**
    - **Diffusion diagram**
        - Takes the necessity of over-propagation into consideration
        - But lacks a good strategy on reducing the propagation range as far as possible
        - Be sensitive to the triangulation quality - may report an inaccurate VD when the base surface does not have a high triangle quality
    - **Sites density**
        - Sparse - fast marching method
        - Dense - straight-line distance
    - Compared with authors’ algorithms, other exact geodesic distance driven GVD algorithms are **hundreds of times slower** than authors’ AGVD or **thousands of times slower** than authors’ SLDBVD
- **Run-time performance**
    - SLDBVD - time complexity - O(m) + O(n)
        - m - the number of sites
        - n - the number of faces
- **Scalability**
    - Surface VDs with breakline constraints
    - Restricted power diagram
    - Curve-type sites
    - VD on a surface with density field
        - In the default setting, the density field is identity
        - But the density field may be the local-feature-size function, shape diameter function, mean curvature, or a kind of importance field
        - A basic requirement is to distribute sample points to adapt to the given density field
        - SLDBVD
            
            ![截屏2022-09-19 11.42.45.png](SurfaceVoronoi%20Efficiently%20Computing%20Surface%20Based%20a7c87449c66d4d6cbcb4e14676aef77c/%25E6%2588%25AA%25E5%25B1%258F2022-09-19_11.42.45.png)
            
            - rho(t) - the density value at the point (1 - t)*v* + *tp* (a point on the line segment *vp*)
            - *d_rho*(*v*,*p*) - the density-equipped distance
            - Can use the squared density-equipped distance to define half-planes, yielding density-equipped VDs
            - The bisectors tend to move toward the regions with larger density

**Conclusion and Limitation**

- **Features**
    - Using the over-propagation mechanism to keep one or more necessary linear distance fields, for each triangle, that may help determine the Voronoi bisectors
    - Using the squared distance to define the linear change of distance in a triangle, which is a provable meaningful extension from the traditional 2D Voronoi diagram to curved surfaces
    - It can work with an arbitrary geodesic algorithm
    - When driven by straight-line distance, is able to deal with thin-sheet models and runs faster than RVD and LRVD
    - It is independent of an existing Voronoi/Delaunay solver and scalable enough to deal with many variant VD problems
- **Limitation**
    - The sweep-based propagation is not easy to be parallelized
    - Not support a triangle soup as the input