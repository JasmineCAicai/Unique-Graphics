# Meshless Deformations Based on Shape Matching

Created: March 16, 2022 10:31 AM
Tags: Literature Reviews

**Title:** Meshless Deformations Based on Shape Matching

**Authors:** Matthias Müller, Bruno Heidelberger, Matthias Teschner, Markus Gross

**Journal:** ACM SIGGRAPH

**Year:** 2005

**Volume:** 24

**Issue:** 3

**Pages:** 471 - 478

---

- **Main idea**
    - Replace energies by geometric constraints and forces by distances of current positions to goal positions
        - These goal positions are determined via a generalized shape matching of an undeformed rest state with the current deformed state of the point cloud.
- **Motivations**
    - Despite the long history of deformable modeling in computer graphics, research results have rarely been applied in computer games.
    - Games are dominated by rigid bodies
    - No practical solution to simulate elastically deformable three-dimensional objects
        - Efficiency
        - Stability
        - Controllability
- **Contributions**
    - Elasticity is modeled by pulling a deformed geometry towards a well-defined goal configuration which is determined by an extended shape matching technique.
    - The degree of representable deformation details can be varied using linear and quadratic deformation modes. Subdivisions into clusters introduce further degrees of freedom.
    - A large variety of objects can be handled.
    - The technique does not require any pre-processing or large accompanying data structures. The configuration of parameters is simple and intuitive.
    - The dynamic simulation is stable under all circumstances and for all deformed geometry configurations.
    - All components of the approach are simple to implement and very efficient in terms of memory requirements and run-time performance.
- **Methodologies (Meshless Animation)**
    - Comparison
        - Implicit integration schemes: stable but computationally expensive
        - Explicit integration schemes: faster to compute but lose unconditional stability
    - Use explicit integration schemes and obtain unconditional stability using a purely geometric scheme
    - Algorithm
        
        ![截屏2022-03-16 下午12.43.49.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%8812.43.49.png)
        
    - Shape Matching
        - x_i^0 and x_i: Two points
        - R: Rotation matrix
        - t and t_0: Translation vector
        - Minimize
            
            ![截屏2022-03-16 下午1.07.16.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%881.07.16.png)
            
        - w_i: Weights of individual points
        - In this paper, w_i = m_i, and the translation vectors turn out to be the center of mass of the initial shape and the center of mass of the actual shape.
            
            ![截屏2022-03-16 下午1.10.40.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%881.10.40.png)
            
        - Compute goal position g_i
            
            ![截屏2022-03-16 下午1.19.15.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%881.19.15.png)
            
    - Integration
        - alpha: simulate stiffness (a belongs to [0...1]
        - Construct the integration scheme
            
            ![截屏2022-03-16 下午1.24.18.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%881.24.18.png)
            
            ![截屏2022-03-16 下午1.28.44.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%881.28.44.png)
            
        - For the one-dimensional spring example,
            
            ![截屏2022-03-16 下午1.29.23.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%881.29.23.png)
            
            ![截屏2022-03-16 下午1.32.03.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%881.32.03.png)
            
    - Discussion
        
        ![截屏2022-03-16 下午1.35.54.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%881.35.54.png)
        
- **Extensions**
    - Rigid body dynamics
    - Linear deformations
    - Quadratic deformations
    - Cluster based deformation
- **Results**
    - Cluster based deformation
        - With an increasing number of clusters, the deformation gets more detailed and physical plausiblity is improved.
        - In general, the number of clusters is user defined. For simple, sphere-like objects, one cluster might be sufficient while for more complex geometries a subdivision might be necessary.
    - Performance
        - The performance scales linearly with the number of points.
        - A larger number of clusters requires more polar decompositions which reduces the performance.
        - The computation of the quadratic deformation model is more involved compared to the linear model.
        - It is difficult to compare the approach to the Finite Element Method or mass-spring models in terms of performance. 因为衡量性能的标准不同，本文提出的方法的性能取决于变形模式和簇的数量。
        
        ![截屏2022-03-16 下午7.53.23.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%887.53.23.png)
        
    - Complex simulation scenarios
        
        ![截屏2022-03-16 下午8.00.09.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%888.00.09.png)
        
    - Interactivity
        
        ![截屏2022-03-16 下午8.00.32.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%888.00.32.png)
        
    - Stability
        
        ![截屏2022-03-16 下午8.00.58.png](Meshless%20D%200947f/%E6%88%AA%E5%B1%8F2022-03-16_%E4%B8%8B%E5%8D%888.00.58.png)
        
- **Future Work**
    - Since this model is not physically motivated and not related to physical vibration modes, this approach gets more involved if additional higher- order deformation modes would be considered.
    - The performance bottleneck of collision handling
    - Fracture animation
        - Fracturing is particularly interesting since changing connectivity requires only minor updates of data structures.