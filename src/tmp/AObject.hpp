#pragma once
#include <SdlContext.hh>
// Inclus la bibliotheque de mathematiques
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// La classe abstraite representant un objet avec sa position, sa rotation et son echelle
class AObject
{
public:
  AObject() :
    _position(0, 0, 0), // On initialise la position a 0
    _rotation(0, 0, 0), // pareil pour la rotation
    _scale(1, 1, 1) // l'echelle est mise a 1
  {
  }
  AObject(int x,int y,int z) :
    _position(x, y, z), // On initialise la position a 0
    _rotation(0, 0, 0), // pareil pour la rotation
    _scale(1, 1, 1) // l'echelle est mise a 1
  {
  }

  virtual ~AObject()
  {}

  // La fonction initialize charge l'objet ou le construit
  virtual bool initialize()
  {
    return (true);
  }

  // La fonction update sert a gerer le comportement de l'objet
  virtual void update(gdl::Clock const &clock, gdl::Input &input)
  {}

  // La fonction draw sert a dessiner l'objet
  virtual void draw(gdl::AShader &shader, gdl::Clock const &clock) = 0;




  void translate(glm::vec3 const &v)
  {
    _position += v;
  }

  void rotate(glm::vec3 const& axis, float angle)
  {
    _rotation += axis * angle;
    if (_rotation.y > 360 || _rotation.y < -360)
      _rotation.y = 0;
  }

  void scale(glm::vec3 const& scale)
  {
    _scale *= scale;
  }

  glm::mat4 getTransformation()
  {
    glm::mat4 transform(1); // On cree une matrice identite

    // On applique ensuite les rotations selon les axes x, y et z
    transform = glm::translate(transform, _position);
    // transform = glm::rotate(transform, _rotation.x, glm::vec3(1, 0, 0)); 


    transform = glm::rotate(transform, _rotation.y, glm::vec3(0, -1, 0));
    // transform = glm::rotate(transform, _rotation.z, glm::vec3(0, 0, 1));
    // On effectue ensuite la translation
    // Et pour finir, on fait la mise a l'echelle
    transform = glm::scale(transform, _scale);
    return (transform);
  }

protected:
  glm::vec3 _position;
  glm::vec3 _rotation;
  glm::vec3 _scale;

};

/////////////////////////     CUBE         //////////////////////////

std::ostream& operator<<(std::ostream& os, glm::vec3& v)
{
  os << "pos x: " << v.x <<  " pos y: ";
  os << v.y <<  "pos z: " << v.z;
  return (os);
}


//Nous allons donc faire à présent notre classe Cube héritant de AObject :

class Cube : public AObject
{
private:
  // La texture utilisee pour le cube
  gdl::Texture _texture;
  // La geometrie du cube
  gdl::Geometry _geometry;
  // La vitesse de deplacement du cube
  float _speed;

public:
  Cube() { }
  Cube(int x, int y, int z): AObject(x,y,z){ }
  virtual ~Cube() { }

  virtual bool initialize()
  {
    //_speed = 2.0f;
    // On charge la texture qui sera affichee sur chaque face du cube
    if (_texture.load("assets/textures/tmp/grass.tga") == false)
      {
	std::cerr << "Cannot load the cube texture" << std::endl;
	return (false);
      }

    _geometry.setColor(glm::vec4(1, 1, 1, 1));

    _geometry.pushVertex(glm::vec3(1, -1, -1));
    _geometry.pushVertex(glm::vec3(1, -1, 1));
    _geometry.pushVertex(glm::vec3(-1, -1, 1));
    _geometry.pushVertex(glm::vec3(-1, -1, -1));

    _geometry.pushUv(glm::vec2(0.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 1.0f));
    _geometry.pushUv(glm::vec2(0.0f, 1.0f));

    // Tres important, on n'oublie pas de build la geometrie pour envoyer ses informations a la carte graphique
    _geometry.build();
    return (true); 
  }


  // Ici le cube bougera avec les fleches du clavier
  virtual void update(gdl::Clock const &clock, gdl::Input &input)
  {
    (void)clock;
    (void)input;
    // On multiplie par le temps ecoule depuis la derniere image pour que la vitesse ne depende pas de la puissance de l'ordinateur
    // if (input.getKey(SDLK_UP))
    //   translate(glm::vec3(0, 0, -1) * static_cast<float>(clock.getElapsed()) * _speed);
    // if (input.getKey(SDLK_DOWN))
    //   translate(glm::vec3(0, 0, 1) * static_cast<float>(clock.getElapsed()) * _speed);
    // if (input.getKey(SDLK_LEFT))
    //   translate(glm::vec3(-1, 0, 0) * static_cast<float>(clock.getElapsed()) * _speed);
    // if (input.getKey(SDLK_RIGHT))
    //   translate(glm::vec3(1, 0, 0) * static_cast<float>(clock.getElapsed()) * _speed);
  }

  virtual void draw(gdl::AShader &shader, gdl::Clock const &clock)
  {
    (void)clock;
    // On bind la texture pour dire que l'on veux l'utiliser
    _texture.bind();
    // Et on dessine notre cube
    _geometry.draw(shader, getTransformation(), GL_QUADS);
  }

};

class Marvin : public AObject
{
private:
  glm::vec3 sc;
  gdl::Model bomberman;
  // La vitesse de deplacement du cube
  float _speed;

public:
  Marvin() {}//: sc(.1,.1,.1){ this->scale(sc);}
  Marvin(int x, int y, int z): AObject(x,y,z), sc(.001,.001,.001){ this->scale(sc);}
  virtual ~Marvin() { }

  virtual bool initialize()
  { 
    _speed = 1.6f;
    
    if (!bomberman.load("lib/LibBomberman_linux_x64/assets/marvin.fbx") ) 
      {
	std::cerr << "Shader error in init function" << std::endl;
	return false;
      }
    return (true);
  }


  // Ici le cube bougera avec les fleches du clavier
  virtual void update(gdl::Clock const &clock, gdl::Input &input)
  {
    glm::vec3 pos = glm::vec3(0, 0, 0);
    float delta = static_cast<float>(clock.getElapsed()) * _speed;
    // On multiplie par le temps ecoule depuis la derniere image pour que la vitesse ne depende pas de la puissance de l'ordinateur
    if (input.getKey(SDLK_UP))
      {
	pos.x = 1;
	_rotation.y = -90;
	translate(pos * delta);  
      }
    if (input.getKey(SDLK_DOWN))
      {
	pos.x = -1;
	_rotation.y = 90;
	translate(pos * delta);
      }
    if (input.getKey(SDLK_LEFT))
      {
	pos.z = -1;
	_rotation.y = 180;
	translate(pos * delta);
      }
    if (input.getKey(SDLK_RIGHT))
      {
	pos.z = 1;
	_rotation.y = 0;
	translate(pos * delta);
      }
  }

  virtual void draw(gdl::AShader &shader, gdl::Clock const &clock)
  {
    (void)clock;
    // On bind la texture pour dire que l'on veux l'utiliser
    // Et on dessine notre cube
    // std::cout << static_cast<float>(clock.getElapsed()) * _speed << std::endl;
    // std::cout << "pos x: " << _position.x <<  " pos y: " << _position.y <<  "pos z: " << _position.z << std::endl;
    std::cout << _rotation << std::endl;
    this->bomberman.draw(shader, getTransformation(), static_cast<float>(clock.getElapsed()));
  }

};

class Camera : public AObject
{
private:
  glm::vec3 sc;
  float _speed;
  glm::mat4 transformation;
public:
  Camera() {}//: sc(.1,.1,.1){ this->scale(sc);}
  Camera(int x, int y, int z): AObject(x,y,z){}
  virtual ~Camera() { }

  virtual bool initialize()
  { 
  }

  void trans(glm::vec3 const &v)
  {
    _position += v;
  }

  void rot(glm::vec3 const& axis, float angle)
  {
    _rotation += axis * angle;
  }

  void scale(glm::vec3 const& scale)
  {
    _scale *= scale;
  }

  glm::mat4 getTrans()
  {
    glm::mat4 transform(1); // On cree une matrice identite

    // On applique ensuite les rotations selon les axes x, y et z
    transform = glm::translate(transform, _position);
    // transform = glm::rotate(transform, _rotation.x, glm::vec3(1, 0, 0)); 


    transform = glm::rotate(transform, _rotation.y, glm::vec3(0, -1, 0));
    // transform = glm::rotate(transform, _rotation.z, glm::vec3(0, 0, 1));
    // On effectue ensuite la translation
    // Et pour finir, on fait la mise a l'echelle
    transform = glm::scale(transform, _scale);
    return (transform);
  }

  // Ici le cube bougera avec les fleches du clavier
  virtual void update(gdl::Clock const &clock, gdl::Input &input)
  {
    glm::vec3 pos = glm::vec3(0, 0, 0);
    float delta = static_cast<float>(clock.getElapsed()) * _speed;
    // On multiplie par le temps ecoule depuis la derniere image pour que la vitesse ne depende pas de la puissance de l'ordinateur
    if (input.getKey(SDLK_e))
      {
	pos.z = 10;
	_rotation.y = 0;
	translate(pos * delta);  
      }
    if (input.getKey(SDLK_d))
      {
	pos.z = -10;
	//_rotation.y = 90;
	trans(pos * delta);
      }
    if (input.getKey(SDLK_s))
      {
	pos.x = -10;
	//_rotation.y = 180;
	trans(pos * delta);
      }
    if (input.getKey(SDLK_f))
      {
	pos.x = 10;
	//_rotation.y = 0;
	trans(pos * delta);
      }
    // transformation = getTrans();
  }

  glm::mat4 Gettransf()
  {
    return (getTrans());
  }
  
  virtual void draw(gdl::AShader &shader, gdl::Clock const &clock)
  {
    (void)clock;
    // On bind la texture pour dire que l'on veux l'utiliser
    // Et on dessine notre cube
    // std::cout << static_cast<float>(clock.getElapsed()) * _speed << std::endl;
    // std::cout << "pos x: " << _position.x <<  " pos y: " << _position.y <<  "pos z: " << _position.z << std::endl;
    std::cout << _position << std::endl;
    // this->bomberman.draw(shader, getTransformation(), static_cast<float>(clock.getElapsed()));
  }

};
