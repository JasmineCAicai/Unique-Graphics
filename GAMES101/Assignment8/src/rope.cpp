#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.

        for (int i = 0; i < num_nodes; i++) {
            Vector2D cur = start + i * (end - start) / (num_nodes - 1);
            Mass* m = new Mass(cur, node_mass, false);
            masses.push_back(m);
        }

        for (int i = 0; i < num_nodes - 1; i++) {
            Spring* s = new Spring(masses[i], masses[i+1], k);
            springs.push_back(s);
        }

        //  Comment-in this part when you implement the constructor
        for (auto &i : pinned_nodes) {
            masses[i]->pinned = true;
        }
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            Vector2D ab = s->m2->position - s->m1->position;
            Vector2D f_ab = s->k * ab.unit() * (ab.norm() - s->rest_length);
            s->m1->forces += f_ab;
            s->m2->forces -= f_ab;
        }

        for (auto &m : masses)
        {
            float kd = 0.1;

            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position

                m->forces += (m->mass * gravity);
                

                // TODO (Part 2): Add global damping

                m->forces += (-kd * m->velocity);

                m->velocity += ((m->forces / m->mass) * delta_t);
                m->position += m->velocity * delta_t;
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)

            Vector2D ab = s->m2->position - s->m1->position;
            Vector2D f_ab = s->k * ab.unit() * (ab.norm() - s->rest_length);
            s->m1->forces += f_ab;
            s->m2->forces -= f_ab;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                m->forces += (m->mass * gravity);
                Vector2D a = m->forces / m->mass;

                // TODO (Part 3.1): Set the new position of the rope mass
                Vector2D temp_position = m->position;

                // TODO (Part 4): Add global Verlet damping
                double damping_factor = 0.00005;
                m->position = m->position +  (1 - damping_factor) * (m->position - m->last_position) + a * delta_t *delta_t;
                m->last_position = temp_position;
            }
            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }
}
