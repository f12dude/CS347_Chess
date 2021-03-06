// -*-c++-*-

#include "Piece.h"
#include "game.h"


Piece::Piece(_Piece* pointer)
{
    ptr = (void*) pointer;
}

int Piece::id() const
{
  return ((_Piece*)ptr)->id;
}

int Piece::owner() const
{
  return ((_Piece*)ptr)->owner;
}

int Piece::file() const
{
  return ((_Piece*)ptr)->file;
}

int Piece::rank() const
{
  return ((_Piece*)ptr)->rank;
}

int Piece::hasMoved() const
{
  return ((_Piece*)ptr)->hasMoved;
}

int Piece::type() const
{
  return ((_Piece*)ptr)->type;
}


int Piece::move(int file, int rank, int type)
{
  return pieceMove( (_Piece*)ptr, file, rank, type);
}



std::ostream& operator<<(std::ostream& stream,Piece ob)
{
  stream << "id: " << ((_Piece*)ob.ptr)->id  <<'\n';
  stream << "owner: " << ((_Piece*)ob.ptr)->owner  <<'\n';
  stream << "file: " << ((_Piece*)ob.ptr)->file  <<'\n';
  stream << "rank: " << ((_Piece*)ob.ptr)->rank  <<'\n';
  stream << "hasMoved: " << ((_Piece*)ob.ptr)->hasMoved  <<'\n';
  stream << "type: " << ((_Piece*)ob.ptr)->type  <<'\n';
  return stream;
}
