# A Quadratic Bending Model for Inextensible Surfaces

Created: March 26, 2022 4:36 PM
Tags: GAMES103 - 基于物理的计算机动画入门, Literature Reviews

Eurographics Symposium Geometry Processing (2006)

---

**Introduction**

- In general the resulting expressions are nonlinear in the positions of mesh vertices, and the attendant numerics involve costly evaluations of energy gradients and Hessians.
- The contribution of this paper is to consider the class of isometric surface deformations, arriving at an expression for bending energy which is quadratic in positions.

The bending energy of a deformable surface *S*:

![截屏2022-05-08 13.41.57.png](A%20Quadratic%20Bending%20Model%20for%20Inextensible%20Surface%206152c73edb7d492f884e390e5ea543be/%E6%88%AA%E5%B1%8F2022-05-08_13.41.57.png)

where *H* is mean curvature and d*A* is the differential area.

![截屏2022-05-08 13.43.07.png](A%20Quadratic%20Bending%20Model%20for%20Inextensible%20Surface%206152c73edb7d492f884e390e5ea543be/%E6%88%AA%E5%B1%8F2022-05-08_13.43.07.png)

<aside>
💡 **Isometric bending model (IBM)**

</aside>

**Discrete IBM**

- To maintain the core properties of the smooth isometric bending model.
- A triangulated surface deforms isometrically if its inner metric does not change, *i.e.*, if all edge lengths remain invariant.
- 
    
    ![截屏2022-05-08 13.50.39.png](A%20Quadratic%20Bending%20Model%20for%20Inextensible%20Surface%206152c73edb7d492f884e390e5ea543be/%E6%88%AA%E5%B1%8F2022-05-08_13.50.39.png)
    
- With quadratic form Q, invariant under isometric deformations, hence depending only on intrinsic mesh properties such as connectivity, edge lengths, inner angles, or area.
- Since Eb is an energy, Q must be positive semi-definite.
- 
    
    ![截屏2022-05-08 13.53.26.png](A%20Quadratic%20Bending%20Model%20for%20Inextensible%20Surface%206152c73edb7d492f884e390e5ea543be/%E6%88%AA%E5%B1%8F2022-05-08_13.53.26.png)
    

**Implementation**

- 
    
    ![截屏2022-05-08 13.55.27.png](A%20Quadratic%20Bending%20Model%20for%20Inextensible%20Surface%206152c73edb7d492f884e390e5ea543be/%E6%88%AA%E5%B1%8F2022-05-08_13.55.27.png)
    
- 
    
    ![截屏2022-05-08 13.59.23.png](A%20Quadratic%20Bending%20Model%20for%20Inextensible%20Surface%206152c73edb7d492f884e390e5ea543be/%E6%88%AA%E5%B1%8F2022-05-08_13.59.23.png)
    
- 
    
    ![截屏2022-05-08 14.00.40.png](A%20Quadratic%20Bending%20Model%20for%20Inextensible%20Surface%206152c73edb7d492f884e390e5ea543be/%E6%88%AA%E5%B1%8F2022-05-08_14.00.40.png)