//Mettons nous maintenant sur la création de notre GameEngine :
#pragma once

#include <Game.hh>
#include <SdlContext.hh>
#include <vector>
#include "AObject.hpp"

/*
  On cree sa class GameEngine qui herite de game
*/
class GameEngine : public gdl::Game
{
public:
  GameEngine():_position(0, 0, 0), // On initialise la position a 0
	       _rotation(0, 0, -1), // pareil pour la rotation
	       _scale(0, 1, 0) // l'echelle est mise a 1

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
    projection = glm::perspective(90.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    // La transformation de la camera correspond a son orientation et sa position
    // La camera sera ici situee a la position 0, 20, -100 et regardera vers la position 0, 0, 0
    transformation = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

    // On doit toujours binder le shader avant d'appeler les methodes setUniform
    _shader.bind();
    _shader.setUniform("view", transformation);
    _shader.setUniform("projection", projection);

    // On va ensuite creer un cube que l'on va ajouter a la liste d'objets

    AObject *cube2 = new Cube(0,0,10);
    AObject *cube3 = new Cube(0,0, 0);
    AObject *cube4 = new Cube(0,0,-10);
   
    AObject *cube22 = new Cube(13,0, 10);
    AObject *cube33 = new Cube(13,0,0);
    AObject *cube44 = new Cube(13,0,-10);

    AObject *cube11 = new Cube(-13,0, 0);
    AObject *cube1 = new Cube(0,0,0);
    
    AObject *marv = new Marvin(0,-1,-5);
    if (cube1->initialize() == false ||
	// cube2->initialize() == false ||
	// cube3->initialize() == false ||
	// cube4->initialize() == false ||
	// cube11->initialize() == false ||
	// cube22->initialize() == false ||
	// cube33->initialize() == false ||
	// cube44->initialize() == false ||

	marv->initialize() == false)
      return (false);
    _objects.push_back(cube1);
    // _objects.push_back(cube2);
    // _objects.push_back(cube3);
    // _objects.push_back(cube4);
    // _objects.push_back(cube11);
    // _objects.push_back(cube22);
    // _objects.push_back(cube33);
    // _objects.push_back(cube44);
    
    _objects.push_back(marv);
    return true;
  }

    void translate(glm::vec3 const &v)
  {
    _position += v;
  }

  void rotate(glm::vec3 const& axis, float angle)
  {
    _rotation += axis * angle;
  }

  void scale(glm::vec3 const& scale)
  {
    _scale *= scale;
  }

  glm::mat4 getTransformation()
  {
    glm::mat4 transform; // On cree une matrice identite

    // On applique ensuite les rotations selon les axes x, y et z
    transform = glm::translate(transform, _position);
    // transform = glm::rotate(transform, _rotation.x, glm::vec3(1, 0, 0)); 


     transform = glm::rotate(transform, _rotation.y, glm::vec3(0, 1, 0));
    // transform = glm::rotate(transform, _rotation.z, glm::vec3(0, 0, 1));
    // On effectue ensuite la translation
    // Et pour finir, on fait la mise a l'echelle
    transform = glm::scale(transform, glm::vec3(1,1,1));
    return (transform);
  }

 bool update()
  {
    
    glm::vec3 pos = glm::vec3(0, 0, 0);

    if (_input.getKey(SDLK_ESCAPE) || _input.getInput(SDL_QUIT))
      return false;
    // Mise a jour des inputs et de l'horloge de jeu
    _context.updateClock(_clock);
    _context.updateInputs(_input);

    float delta = static_cast<float>(_clock.getElapsed()) * 1.0f;

    // On multiplie par le temps ecoule depuis la derniere image pour que la vitesse ne depende pas de la puissance de l'ordinateur
    if (_input.getKey(SDLK_UP))
      {
	pos.z = -1;
	//_rotation.y = 0;
	translate(pos * delta);  
      }
    if (_input.getKey(SDLK_DOWN))
      {
	pos.z = 1;
	//_rotation.y = 0;
	translate(pos * delta);
      }
    if (_input.getKey(SDLK_LEFT))
      {
	pos.y = 1;
	//_rotation.y = 10;
	//translate(pos * delta);
	rotate(pos, 1.0f);
      }
    if (_input.getKey(SDLK_RIGHT))
      {
	pos.y = -1;
	//_rotation.y = -10;
	//translate(pos * delta);
	rotate(pos, 1.0f);
      }
    return (true);
  }

  void draw()
  {
    // On clear l'ecran
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // pour utiliser un shader (pour que ce soit ce dernier qui dessine la geometrie) il faut le binder.
    // Un seul shader peut etre actif en meme temps
    _shader.setUniform("view", getTransformation());
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
  glm::vec3 _position;
  glm::vec3 _rotation;
  glm::vec3 _scale;
  gdl::SdlContext _context;
  gdl::Clock _clock;
  gdl::Input _input;
  gdl::BasicShader _shader;
  std::vector<AObject*> _objects;

};
