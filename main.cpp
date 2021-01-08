#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <stack>
#include <opencv2/opencv.hpp>

#define MARGIN	38
#define WALL	0
#define WAY		255

const int dy[] = { 1, 0, -1, 0 };
const int dx[] = { 0, 1, 0, -1 };

struct Pos
{
	int x;
	int y;

	Pos(int X, int Y)
		:x(X), y(Y)
	{}
};

/*
void dfs(int x, int y, int width, int height)
{
	static std::vector<std::vector<bool>> visited(height, std::vector<bool>(width));

	for (int i = 0; i < 4; i++)
	{
		Pos next = Pos(x + dx[i], y + dy[i]);

		if (next.x < (width - MARGIN) && next.y < (height - MARGIN) && next.x >= MARGIN && next.y >= MARGIN
			&& grayMaze.at<uchar>(next.x, next.y) != WALL && visited[next.y][next.x] != true)
		{
			grayMaze.at<uchar>(next.x, next.y) = 100;

			cv::imshow("Grayscale", grayMaze);
			cv::waitKey(1);
			dfs(next.x, next.y, width, height);
		}
	}
	visited[x][y] = true;
}
*/

int main()
{
	cv::Mat orignMaze = cv::imread("maze.png", cv::IMREAD_COLOR);
	if (orignMaze.empty())
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	cv::Mat grayMaze;
	cvtColor(orignMaze, grayMaze, cv::COLOR_BGR2GRAY);

	int height = orignMaze.size().height;
	int width = orignMaze.size().width;

	//std::vector<std::vector<int>> map(height, std::vector<int>(width));
	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width));
	std::vector<std::vector<int>> trace(height, std::vector<int>(width));

	std::queue<Pos> q;

	for (int i = MARGIN; i < width; i++)
	{
		if (grayMaze.at<uchar>(MARGIN, i) == WAY)
		{
			q.push(Pos(MARGIN, i));
			break;
		}
	}

	int k = 0;
	int prog = 0;
	while (!q.empty())
	{
		Pos p = q.front();
		q.pop();

		for (int i = 0; i < 4; i++)
		{
			Pos next(p.x + dx[i], p.y + dy[i]);

			if (next.x <= (width - MARGIN) && next.y <= (height - MARGIN) && next.x >= MARGIN && next.y >= MARGIN
				&& grayMaze.at<uchar>(next.x, next.y) != WALL && visited[next.y][next.x] != true)
			{
				q.push(next);
				visited[next.y][next.x] = true;

				trace[next.y][next.x] = trace[p.y][p.x] + 1;
				//grayMaze.at<uchar>(next.x, next.y) = 100;
			}
		}

		//if (k % 4650 == 0)
		//	printf("%d%% \r", prog++);

		k++;
	}

	q.push(Pos(761, 762));
	std::stack<Pos> path;

	while (!q.empty())
	{
		Pos p = q.front();
		q.pop();
		path.push(p);

		for (int i = 0; i < 4; i++)
		{
			Pos next(p.x + dx[i], p.y + dy[i]);

			if (trace[next.y][next.x] == trace[p.y][p.x] - 1)
			{
				q.push(next);
				break;
			}
		}
	}

	cv::namedWindow("Grayscale", cv::WINDOW_AUTOSIZE);
	while (!path.empty())
	{
		Pos p = path.top();
		path.pop();

		//grayMaze.at<uchar>(p.x, p.y) = 100;
		cv::rectangle(grayMaze, cv::Point(p.y, p.x), cv::Point(p.y - 5, p.x - 5), 100, 3);
		cv::imshow("Grayscale", grayMaze);
		cv::waitKey(1);
	}

	cv::waitKey(0);
	return 0;
}