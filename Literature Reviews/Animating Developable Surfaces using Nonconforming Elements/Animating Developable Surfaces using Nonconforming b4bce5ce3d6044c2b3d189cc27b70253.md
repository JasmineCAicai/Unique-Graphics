# Animating Developable Surfaces using Nonconforming Elements

Created: March 26, 2022 4:37 PM
Tags: GAMES103 - 基于物理的计算机动画入门, Literature Reviews

Authors: Elliot English, Robert Bridson

Journal: ACM Transactions on Graphics

Vol. 27

No. 3

Article 66

Publication date: August 2008.

---

<aside>
💡 **Solve locking issue**

</aside>

---

Constrained to not deform at all in-plane but free to bend out-of-plane, these are an excellent approximation for many materials, including most cloth, paper, and stiffer materials.

Unfortunately the conforming (geometrically continuous discretizations used in graphics break down in this limit.

The method proposed in this paper solves this problem, allowing us to **simulate surfaces with zero in-plane deformation as a hard constraint**.

But this method will **produces discontinuous meshes**, so the authors further couple this with a “ghost” conforming mesh for collision processing and rendering.

The authors propose a new second order accurate constrained mechanics time integration method that greatly reduces the numerical damping present in the usual first order methods used in graphics, for virtually no extra cost and sometimes significant speed-up.

- Restrict attention to the fully developable case
- Impose zero in-plane deformation as a hard constraint
- For any nontrivial bending this constraint must be violated: the mesh can essentially only crease along straight lines already present in the mesh as edges: it locks.

For locking problem, rather than reduce the number of constraints by averaging over larger regions, finite element practitioners traditionally increase the number of variables, by putting the variables at the midpoints of edges (in 2D) or faces (in 3D).

- The authors adopt this approach for the deformable surface case, **putting the position variables at the midpoints of the edges rather than the vertices in the mesh.**
    
    ![截屏2022-05-13 15.07.57.png](Animating%20Developable%20Surfaces%20using%20Nonconforming%20b4bce5ce3d6044c2b3d189cc27b70253/%E6%88%AA%E5%B1%8F2022-05-13_15.07.57.png)
    

Since they impose developability as a hard constraint, they turn to time integration of constrained mechanics.

- The usual schemes in graphics unfortunately suffer from strong numerical damping with nonlinear constraints, as energy is erroneously transferred to constrained modes and projected out.
- They propose a new second order accurate multistep method, based on BDF2 and simple position-based constraint projection.
    - This both reduces numerical damping and speeds projection, without need for stabilization or velocity projection.

### A Non Conforming Element Discretization

![截屏2022-05-13 15.23.43.png](Animating%20Developable%20Surfaces%20using%20Nonconforming%20b4bce5ce3d6044c2b3d189cc27b70253/%E6%88%AA%E5%B1%8F2022-05-13_15.23.43.png)

![截屏2022-05-13 15.24.10.png](Animating%20Developable%20Surfaces%20using%20Nonconforming%20b4bce5ce3d6044c2b3d189cc27b70253/%E6%88%AA%E5%B1%8F2022-05-13_15.24.10.png)

![截屏2022-05-13 15.24.38.png](Animating%20Developable%20Surfaces%20using%20Nonconforming%20b4bce5ce3d6044c2b3d189cc27b70253/%E6%88%AA%E5%B1%8F2022-05-13_15.24.38.png)

**Boundary Constraints**

**Bending Forces**

### Collisions and Rendering

### Time Integration of Constraints

### **Results**

![截屏2022-05-13 15.30.02.png](Animating%20Developable%20Surfaces%20using%20Nonconforming%20b4bce5ce3d6044c2b3d189cc27b70253/%E6%88%AA%E5%B1%8F2022-05-13_15.30.02.png)