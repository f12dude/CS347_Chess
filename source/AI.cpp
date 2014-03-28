#include "AI.h"
#include "Timer.h"
#include "Heuristics.h"

#include <algorithm>
#include <cassert>
#include <cfloat>
#include <functional>

using std::cout;
using std::endl;
using namespace std::placeholders;

AI::AI(Connection* conn, unsigned int depth) : BaseAI(conn), m_totalTime(0), m_count(1), m_depth(depth) {}

const char* AI::username()
{
	return "Cole Xemi";
}

const char* AI::password()
{
	return "password";
}

//This function is run once, before your first turn.
void AI::init()
{
}

//This function is called each time it is your turn.
//Return true to end your turn, return false to ask the server for updated information.
bool AI::run()
{
#ifdef DEBUG_OUTPUT
	Timer timer;
	timer.Start();
#endif

#ifdef DEBUG_OUTPUT
	DrawBoard();
#endif

	m_board.Update(TurnsToStalemate(), moves, pieces);
	BoardMove bestMove;
	if(MiniMax(bestMove))
	{
		BoardPiece* pPiece = m_board.GetPiece(bestMove.from);
		assert(pPiece != nullptr);
		pPiece->piece.move(bestMove.to.x, bestMove.to.y, bestMove.promotion);
	}

#ifdef DEBUG_OUTPUT
	m_totalTime += timer.GetTime();
	cout << "Average Time Spent: " << m_totalTime / m_count << endl;
	cout << "Total Time Spent: " << m_totalTime << endl;
	cout << "Servers time: " << 900 - players[playerID()].time() << endl;

	m_count++;
#endif

	return true;
}

//This function is run once, after your last turn.
void AI::end()
{
}

bool AI::MiniMax(BoardMove& moveOut)
{
	bool bFoundMove = false;
	unsigned int i = 1;

	m_minimaxTimer.Reset();
	m_minimaxTimer.Start();

	while(i <= m_depth && ((m_minimaxTimer.GetTime()) < 9000000000))
	{
		bFoundMove |= MiniMax(i, playerID(), moveOut);
		cout << "Depth " << i << " time: " << m_minimaxTimer.GetTime() << endl;
		++i;
	}

	return bFoundMove;
}

bool AI::MiniMax(int depth, int playerID, BoardMove& moveOut)
{
	unsigned int index = 0;
	float bestVal = -FLT_MAX;
	bool bFoundMove = false;

	std::vector<BoardMove> userMoves =  m_board.GetMoves(playerID);
	for(unsigned int i = 0; i < userMoves.size(); ++i)
	{
		ApplyMove theMove(&userMoves[i], &m_board);

		float val = -MiniMax(depth - 1, playerID, -FLT_MAX, FLT_MAX, -1);

		// If the new move is better than the last
		if(val > bestVal)
		{
			index = i;
			bestVal = val;
			bFoundMove = true;
		}

		// If we have ran out of time
		if(bFoundMove && ((m_minimaxTimer.GetTime()) >= 9000000000))
		{
			bFoundMove = false;
			break;
		}
	}

	if(bFoundMove)
	{
		moveOut = userMoves[index];
	}

	return bFoundMove;

}

float AI::MiniMax(int depth, int playerID, float a, float b, int color)
{
	if(m_board.IsInCheckmate(!playerID))
		return color*10000.0f;

	if(m_board.IsInStalemate(color == 1 ? playerID : !playerID))
		return -color*500.0f;

	if(depth <= 0)
		return color*m_board.GetWorth(playerID, ChessHeuristic());

	float bestValue = -FLT_MAX;

	const std::vector<BoardMove> enemyMoves = m_board.GetMoves(color == 1 ? !playerID : playerID);
	std::vector<BoardMove> userMoves =  m_board.GetMoves(color == 1 ? playerID : !playerID);
	std::partition(userMoves.begin(), userMoves.end(),[&](const BoardMove& a) -> bool
	{
		if(a.capturedType != 0)
			return true;

		bool bUnderAttack = false;
		for(const BoardMove& m : enemyMoves)
		{
			if(m.to == a.from)
			{
				bUnderAttack = true;
				break;
			}
		}

		return bUnderAttack;
	});
	for(unsigned int i = 0; i < userMoves.size(); ++i)
	{
		ApplyMove theMove(&userMoves[i], &m_board);

		bestValue = std::max(bestValue, -MiniMax(depth - 1, playerID, -b, -a, -color));
		a = std::max(a, bestValue);

		if(a >= b)
		{
			break;
		}
	}

	return bestValue;
}

void AI::DrawBoard() const
{
	// Print out the current board state
	cout<<"+---+---+---+---+---+---+---+---+"<<endl;
	for(int rank=8; rank>0; rank--)
	{
		cout<<"|";
		for(int file=1; file<=8; file++)
		{
			bool found = false;
			// Loops through all of the pieces
			for(unsigned int p=0; !found && p<pieces.size(); p++)
			{
				// determines if that piece is at the current rank and file
				if(pieces[p].rank() == rank && pieces[p].file() == file)
				{
					found = true;
					// Checks if the piece is black
					if(pieces[p].owner() == 1)
					{
						cout<<"*";
					}
					else
					{
						cout<<" ";
					}
					// prints the piece's type
					cout<<(char)pieces[p].type()<<" ";
				}
			}

			if(!found)
			{
				cout<<"   ";
			}
			cout<<"|";
		}
		cout<<endl<<"+---+---+---+---+---+---+---+---+"<<endl;
	}
}
