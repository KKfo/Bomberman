//Mettons nous maintenant sur la création de notre GameEngine :
#pragma once

#include <Game.hh>
#include <SdlContext.hh>
#include <vector>
#include "AObject.hpp"

/*
  On cree sa class GameEngine qui herite de game
*/

Camera cam;

class GameEngine : public gdl::Game
{
public:
  GameEngine()
  {
  }

  bool initialize()
  {
    if (!_context.start(1920, 1080, "My bomberman!")) // on cree une fenetre
      return false;

    // On active le test de profondeur d'OpenGL pour que les pixels que l'oeil ne voit pas ne s'affichent pas
    glEnable(GL_DEPTH_TEST);

    // On cree un shader, petit programme permettant de dessiner nos objets a l'ecran 


    if (!_shader.load("lib/LibBomberman_linux_x64/shaders/basic.fp", GL_FRAGMENT_SHADER) // le fragment shader se charge de dessiner les pixels
	|| !_shader.load("lib/LibBomberman_linux_x64/shaders/basic.vp", GL_VERTEX_SHADER) // le vertex shader s'occupe de projeter les points sur l'ecran
	|| !_shader.build()) // il faut ensuite compiler son shader
      {
	std::cerr << "Shader error in init function" << std::endl;
	return false;
      }
 
    // On place ensuite la camera (sa projection ainsi que sa transformation)
    glm::mat4 projection;
    glm::mat4 transformation;

    // La projection de la camera correspond a la maniere dont les objets vont etre dessine a l'ecran
    projection = glm::perspective(90.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
    // La transformation de la camera correspond a son orientation et sa position
    // La camera sera ici situee a la position 0, 20, -100 et regardera vers la position 0, 0, 0
    transformation = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

    // On doit toujours binder le shader avant d'appeler les methodes setUniform
    _shader.bind();
    _shader.setUniform("view", transformation);
    _shader.setUniform("projection", projection);

    // On va ensuite creer un cube que l'on va ajouter a la liste d'objets

    AObject *floor = new Floor();
    
    AObject *marv = new Marvin(0,-1,-10);
    if (floor->initialize() == false ||
	marv->initialize() == false)
      return (false);
    _objects.push_back(floor);
    _objects.push_back(marv);
    return true;
  }

  bool update()
  {
    // Si la touche ECHAP est appuyee ou si l'utilisateur ferme la fenetre, on quitte le programme
    if (_input.getKey(SDLK_ESCAPE) || _input.getInput(SDL_QUIT))
      return false;
    // Mise a jour des inputs et de l'horloge de jeu
    _context.updateClock(_clock);
    _context.updateInputs(_input);
    // Mise a jour des differents objets
    for (size_t i = 0; i < _objects.size(); ++i)
      _objects[i]->update(_clock, _input);
    cam.update(_clock, _input);
    return true;
  }

  void draw()
  {
    // On clear l'ecran
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // pour utiliser un shader (pour que ce soit ce dernier qui dessine la geometrie) il faut le binder.
    // Un seul shader peut etre actif en meme temps
    _shader.setUniform("view", cam.getTrans());
    _shader.bind();
 

    // On dessine tous les objets composant la scene
    for (size_t i = 0; i < _objects.size(); ++i)
      _objects[i]->draw(_shader, _clock);
    
    // On met ensuite a jour l'ecran pour que ce que l'on a dessine s'affiche
    _context.flush();
  }

  ~GameEngine()
  {
    // N'oublions pas de supprimer les objets une fois le programme termine!
    for (size_t i = 0; i < _objects.size(); ++i)
      delete _objects[i];
  }

private:
  gdl::SdlContext _context;
  gdl::Clock _clock;
  gdl::Input _input;
  gdl::BasicShader _shader;
  std::vector<AObject*> _objects;

};
