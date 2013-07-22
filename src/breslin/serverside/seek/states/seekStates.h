#ifndef SEEKTATES_H
#define SEEKSTATES_H

#include "../../../fsm/state.h"

class Seek;
struct Telegram;


/*******************************
*      GLOBAL_SEEK
******************************/

class GLOBAL_SEEK : public State<Seek>
{
private:
  GLOBAL_SEEK(){}
public:
  static GLOBAL_SEEK* Instance();
  void enter  (Seek* seek);
  void execute(Seek* seek);
  void exit   (Seek* seek);
  bool onLetter(Seek* seek, Letter* letter);
};

/*******************************
*      Seek
******************************/

class NORMAL_SEEK : public State<Seek>
{
private:
  NORMAL_SEEK(){}
public:
  static NORMAL_SEEK* Instance();
  void enter  (Seek* seek);
  void execute(Seek* seek);
  void exit   (Seek* seek);
  bool onLetter(Seek* seek, Letter* letter);
};

class Reached_Destination : public State<Seek>
{
private:
  Reached_Destination(){}
public:
  static Reached_Destination* Instance();
  void enter  (Seek* seek);
  void execute(Seek* seek);
  void exit   (Seek* seek);
  bool onLetter(Seek* seek, Letter* letter);
};


class NO_SEEK : public State<Seek>
{
private:
  NO_SEEK(){}
public:
  static NO_SEEK* Instance();
  void enter  (Seek* seek);
  void execute(Seek* seek);
  void exit   (Seek* seek);
  bool onLetter(Seek* seek, Letter* letter);
};

#endif
