#pragma once

#include "Barrier.h"
#include "Scene.h"
#include "Vec3D.h"
#include "Color.h"
#include "Rayon.h"
#include "Sphere.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <sstream>

namespace pr
{

    class Job
    {
    protected:
        Barrier &barrier;

    public:
        Job(Barrier &b) : barrier(b) {}
        // le = 0 oblige aux types concrets de Job à implémenter run()
        virtual void run() = 0;
        // virtual sur le destructeur pour permettre la destruction polymorphique
        // c'est à dire que si on a un pointeur de type Job qui pointe sur un objet
        // de type SleepJob, on veut que le destructeur de SleepJob soit appelé
        virtual ~Job() {};
    };

    // Job concret : exemple

    class SleepJob : public Job
    {
    private:
        int arg;
        int *ret;

        int calcul(int v)
        {
            std::stringstream msg;

            msg << "Computing for arg =" << v << std::endl;
            std::cout << msg.str();
            msg.clear();

            // traiter un gros calcul
            std::this_thread::sleep_for(std::chrono::seconds(1));
            int ret = v % 255;

            msg << "Obtained for arg =" << arg << " result " << ret << std::endl;
            std::cout << msg.str();
            return ret;
        }

    public:
        SleepJob(int arg, int *ret, Barrier &barrier) : Job(barrier), arg(arg), ret(ret) {}

        void run() override
        {
            *ret = calcul(arg);
            barrier.done();
        }
        ~SleepJob() override = default;
    };

    class LinePixelJob : public Job
    {
    private:
        const pr::Scene &scene;
        const pr::Scene::screen_t &screen;
        const std::vector<pr::Vec3D> &lights;
        pr::Color *pixels;
        int lineNumber;
        int width;

        int findClosestInter(const Rayon &ray) const
        {
            auto minz = std::numeric_limits<float>::max();
            int targetSphere = -1;
            int index = 0;
            for (const auto &obj : scene)
            {
                // rend la distance de l'objet a la camera
                auto zinter = obj.intersects(ray);
                // si intersection plus proche  ?
                if (zinter < minz)
                {
                    minz = zinter;
                    targetSphere = index;
                }
                index++;
            }
            return targetSphere;
        }

        // Calcule l'angle d'incidence du rayon à la sphere, cumule l'éclairage des lumières
        // En déduit la couleur d'un pixel de l'écran.
        Color computeColor(const Sphere &obj, const Rayon &ray, const Vec3D &camera) const
        {
            Color finalcolor = obj.getColor();

            // calcul du rayon et de sa normale a la sphere
            // on prend le vecteur de la camera vers le point de l'ecran (dest - origine)
            // on le normalise a la longueur 1, on multiplie par la distance à l'intersection
            Vec3D rayInter = (ray.dest - ray.ori).normalize() * obj.intersects(ray);
            // le point d'intersection
            Vec3D intersection = rayInter + camera;
            // la normale a la sphere au point d'intersection donne l'angle pour la lumiere
            Vec3D normal = obj.getNormale(intersection);
            // le niveau d'eclairage total contribue par les lumieres 0 sombre 1 total lumiere
            double dt = 0;
            // modifier par l'eclairage la couleur
            for (const auto &light : lights)
            {
                // le vecteur de la lumiere au point d'intersection
                Vec3D tolight = (light - intersection);
                // si on est du bon cote de la sphere, i.e. le rayon n'intersecte pas avant de l'autre cote
                if (obj.intersects(Rayon(light, intersection)) >= tolight.length() - 0.05)
                {                                       //  epsilon 0.05 for double issues
                    dt += tolight.normalize() & normal; // l'angle (scalaire) donne la puissance de la lumiere reflechie
                }
            }
            // eclairage total
            finalcolor = finalcolor * dt + finalcolor * 0.2; // *0.2 = lumiere speculaire ambiante

            return finalcolor;
        }

        void processLine()
        {
            for (int x = 0; x < width; ++x)
            {
                pr::Rayon ray(scene.getCameraPos(), screen[lineNumber][x]);
                int targetSphere = findClosestInter(ray);
                if (targetSphere != -1)
                {
                    const pr::Sphere &obj = *(scene.begin() + targetSphere);
                    pr::Color finalcolor = computeColor(obj, ray, scene.getCameraPos());
                    pixels[lineNumber * width + x] = finalcolor;
                }
            }
        }

    public:
        LinePixelJob(const pr::Scene &scene, const pr::Scene::screen_t &screen,
                     const std::vector<pr::Vec3D> &lights, pr::Color *pixels,
                     int lineNumber, int width, pr::Barrier &barrier)
            : Job(barrier), scene(scene), screen(screen), lights(lights),
              pixels(pixels), lineNumber(lineNumber), width(width) {}

        void run() override
        {
            processLine();
            barrier.done();
        }
    };

}