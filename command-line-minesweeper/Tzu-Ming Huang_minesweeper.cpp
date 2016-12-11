#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <queue>
#include <algorithm>

enum brick_state { mine, number, blank };
enum action { flip, mark };

class Cbrick;
class CMineSweeper;

class Cbrick
{
public:
	Cbrick();
	bool m_flip();
	bool m_mark();
	bool m_is_flipped(){ return bflipped; }
	bool m_is_marked(){ return bmarked; }
	brick_state m_state(){ return state; }
	int m_get_number(){ return inumber_of_mine_around; }
	void m_set_state(brick_state s){ state = s; }
	void m_add_number(int i_num){ inumber_of_mine_around += i_num; }

private:
	brick_state state;
	bool bflipped;
	bool bmarked;
	int inumber_of_mine_around;
};

Cbrick::Cbrick()
{
	state = blank;
	bflipped = false;
	bmarked = false;
	inumber_of_mine_around = 0;
}
bool Cbrick::m_flip()
{
	if (bflipped || bmarked)
		return false;
	else
	{
		bflipped = true;
		return true;
	}
}
bool Cbrick::m_mark()
{
	if (!bflipped)
	{
		bmarked = !bmarked;
		return true;
	}
	else
		return false;
}

class CMineSweeper
{
public:
	CMineSweeper(int i_sizex, int i_sizey, int i_mine);
	~CMineSweeper();
	void initial_board();
	void processInput();
	void update();
	void render();
private:
	int inumber_of_mine, itotal_flipped;
	std::vector < std::vector <Cbrick> > bricks;
	std::vector < std::vector <Cbrick> >::iterator brick_column;
	std::vector <Cbrick>::iterator brick_row;
	std::queue < std::pair < std::vector <Cbrick>::iterator, action> > input_action;
	void m_show_blanks();
	void m_show_mines();
};

CMineSweeper::CMineSweeper(int i_sizex, int i_sizey, int i_mine)
{
	itotal_flipped = 0;
	inumber_of_mine = i_mine;
	bricks.resize(i_sizey);
	for (auto &v : bricks)
		v.resize(i_sizex);
}
CMineSweeper::~CMineSweeper()
{
	bricks.clear();
}
void CMineSweeper::initial_board()
{
	srand(time(NULL));

	// generate mine
	for (int i = 0; i < inumber_of_mine; ++i)
	{
		std::vector < std::vector <Cbrick> >::iterator col;// = bricks.begin();
		std::vector <Cbrick>::iterator row;// = col->begin();
		int rand_col, rand_raw;
		do
		{
			rand_col = (rand() % bricks.size());
			col = bricks.begin() + rand_col;
			rand_raw = (rand() % col->size());
			row = col->begin() + rand_raw;
		}while((&(*row) == &(*brick_row)) || (row->m_state() == mine));
		
		row->m_set_state(mine);		
	}

	// generate number
	for (auto col = bricks.begin(); col < bricks.end(); ++col)
	{
		auto row = col->begin();
		for (int i = 0; ; ++i)
		{
			row = col->begin() + i;
			if (row >= col->end())
				break;
			if (row->m_state() == mine)
			{
				if ((row - 1) >= col->begin())
				{
					if ((row - 1)->m_state() != mine)
					{
						(row - 1)->m_set_state(number);
						(row - 1)->m_add_number(1);
					}
				}
				if ((row + 1) < col->end())
				{
					if ((row + 1)->m_state() != mine)
					{
						(row + 1)->m_set_state(number);
						(row + 1)->m_add_number(1);
					}
				}

				if ((col - 1) >= bricks.begin())
				{
					row = (col - 1)->begin() + i;
					if ((row - 1) >= (col - 1)->begin() && (row - 1)->m_state() != mine)
					{
						(row - 1)->m_set_state(number);
						(row - 1)->m_add_number(1);
					}
					if (row->m_state() != mine)
					{
						row->m_set_state(number);
						row->m_add_number(1);
					}
					if ((row + 1) < (col - 1)->end() && (row + 1)->m_state() != mine)
					{
						(row + 1)->m_set_state(number);
						(row + 1)->m_add_number(1);
					}
				}

				if ((col + 1) < bricks.end())
				{
					row = (col + 1)->begin() + i;
					if ((row - 1) >= (col + 1)->begin() && (row - 1)->m_state() != mine)
					{
						(row - 1)->m_set_state(number);
						(row - 1)->m_add_number(1);
					}
					if (row->m_state() != mine)
					{
						row->m_set_state(number);
						row->m_add_number(1);
					}
					if ((row + 1) < (col + 1)->end() && (row + 1)->m_state() != mine)
					{
						(row + 1)->m_set_state(number);
						(row + 1)->m_add_number(1);
					}
				}
			}
		}
	}
}
void CMineSweeper::processInput()
{
	int col, row, act;
	std::cin >> col >> row >> act;

	// forward checking
	if (bricks.empty() || (col >= bricks.size()) || (row >= bricks.begin()->size()))
		return;
	else
	{
		brick_column = bricks.begin() + col;
		brick_row = brick_column->begin() + row;
	}

	// third input: 0 = left click, 1 = right click
	switch (act)
	{
		case 0:
			input_action.push(std::make_pair(brick_row, flip));
			break;
		case 1:
			input_action.push(std::make_pair(brick_row, mark));
			break;
	}
}
void CMineSweeper::update()
{
	while (!input_action.empty())
	{
		auto act = input_action.front();
		input_action.pop();

		switch (act.second)
		{
			case flip:
				if (act.first->m_flip())
				{
					++ itotal_flipped;
					switch (act.first->m_state())
					{
						case mine:
						// show "lose" and reveal all mine on board
							std::cout << "YOU LOSE ! ! !" << std::endl;
							m_show_mines();
							exit(0);
							break;
						case blank:
						// Breadth-First-Search  reveal the blank area coverd by numbers
							m_show_blanks();
							break;
					}
				}
				break;
			case mark:
				act.first->m_mark();
				break;
		}
	}

	// check goal state
		if ((itotal_flipped + inumber_of_mine) == (bricks.size() * (bricks.at(0).size())))
		// win
		{
			std::cout << "YOU WIN ! ! !" << std::endl;
			exit(0);
		}
}
void CMineSweeper::render()
{
	for (auto v : bricks)
	{
		std::cout << "|";
		for (auto b : v)
		{
			if (!b.m_is_flipped())
			{
				if (b.m_is_marked())
					std::cout << "#" << "|";
				else
					std::cout << "\u25A0" << "|";
			}
			else
			{
				switch (b.m_state())
				{
					case mine:
						std::cout << "*" << "|";
						break;
					case number:
						std::cout << b.m_get_number() << "|";
						break;
					case blank:
						std::cout << " " << "|";
						break;
				}
			}
		}
		std::cout << std::endl;
	}
}
void CMineSweeper::m_show_blanks()
{
	std::queue < std::pair < std::vector < std::vector <Cbrick> >::iterator, std::vector <Cbrick>::iterator > > BFSqueue;
	std::vector < std::vector <Cbrick>::iterator > BFSblanks, BFSvisited;

	// first element
	if (brick_row->m_state() == blank)
	{
		BFSqueue.push(std::make_pair(brick_column, brick_row));
		BFSvisited.push_back(brick_row);
	}
	else
		return;

	// BFS algorithm start
	while (!BFSqueue.empty())
	{
		auto brick_c = BFSqueue.front().first;
		auto brick_r = BFSqueue.front().second;
		BFSqueue.pop();	// pop after getting element

		if (brick_r->m_state() == blank)
		{
			BFSblanks.push_back(brick_r);
			if ((brick_c - 1) >= bricks.begin())
			{
				bool finded = false;
				auto brick_t_row = (brick_c - 1)->begin() + (brick_r - brick_c->begin());
				for (auto v : BFSvisited)
				{
					if (std::distance(brick_t_row, v) == 0)
					{
						finded = true;
						break;
					}
				}
				if (!finded)
				{
					BFSqueue.push(std::make_pair((brick_c - 1), brick_t_row));
					BFSvisited.push_back(brick_t_row);
				}
			}

			if ((brick_c + 1) < bricks.end())
			{
				bool finded = false;
				auto brick_t_row = (brick_c + 1)->begin() + (brick_r - brick_c->begin());
				for (auto v : BFSvisited)
				{
					if (std::distance(brick_t_row, v) == 0)
					{
						finded = true;
						break;
					}
				}
				if (!finded)
				{
					BFSqueue.push(std::make_pair((brick_c + 1), brick_t_row));
					BFSvisited.push_back(brick_t_row);
				}
			}

			if ((brick_r - 1) >= brick_c->begin())
			{
				bool finded = false;
				auto brick_t_row = (brick_r - 1);
				for (auto v : BFSvisited)
				{
					if (std::distance(brick_t_row, v) == 0)
					{
						finded = true;
						break;
					}
				}
				if (!finded)
				{
					BFSqueue.push(std::make_pair(brick_c, brick_t_row));
					BFSvisited.push_back(brick_t_row);
				}
			}

			if ((brick_r + 1) < brick_c->end())
			{
				bool finded = false;
				auto brick_t_row = (brick_r + 1);
				for (auto v : BFSvisited)
				{
					if (std::distance(brick_t_row, v) == 0)
					{
						finded = true;
						break;
					}
				}
				if (!finded)
				{
					BFSqueue.push(std::make_pair(brick_c, brick_t_row));
					BFSvisited.push_back(brick_t_row);
				}
			}
		}
		else if (brick_r->m_state() == number)
		{
			BFSblanks.push_back(brick_r);
		}
	}
	
	for (auto v : BFSblanks)
		itotal_flipped += v->m_flip();
}
void CMineSweeper::m_show_mines()
{
	for (auto &v : bricks)
	{
		for (auto &b : v)
		{
			if (b.m_state() == mine)
				b.m_flip();
		}
	}
	render();
}


int main(int argc, char *argv[])
{
	// width, height of board and number of mine
	if (argc != 4)
	{
		std::cerr << "command should contain 3 arguments: width, height, and number_of_mines" << std::endl;
		return 1;
	}

	CMineSweeper game(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
	game.render();

	game.processInput();
	game.initial_board();
	game.update();
	game.render();

	while(1)
	{
		game.processInput();
		game.update();
		game.render();
	}

	return 0;
}