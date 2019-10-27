#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

typedef enum Rating
{
	T,
	E,
	M,
	E10,
}Rating;

double Dist_Sqrt(int x, int y, float z,
				int a, int b, float c)
{
	int x_sq = (x - a) * (x - a);
	int y_sq = (y - b) * (y - b);
	double z_sq = double((z - c)) * double((z - c));
	return static_cast<double>(x_sq + y_sq + z_sq);
}
Rating CheckPrediction(std::vector<std::tuple<double, Rating>>& results, int k)
{
	int t_count = 0;
	int e_count = 0;
	int m_count = 0;
	int e_10_count = 0;
	for (int i = 0; i < k; ++i)
	{
		if (std::get<1>(results[i]) == T)
			++t_count;
		else if (std::get<1>(results[i]) == E)
			++e_count;
		else if (std::get<1>(results[i]) == M)
			++m_count;
		else if (std::get<1>(results[i]) == E10)
			++e_10_count;
	}
	int max = t_count;
	if (max < e_count)
		max = e_count;
	if (max < m_count)
		max = m_count;
	if (max < e_10_count)
		max = e_10_count;
	std::cout << "T : " << t_count << std::endl;
	std::cout << "E : " << e_count << std::endl;
	std::cout << "M : " << m_count << std::endl;
	std::cout << "E10 : " << e_10_count << std::endl;
	Rating prediction;
	if (max == t_count)
		prediction = T;
	else if (max == e_count)
		prediction = E;
	else if (max == m_count)
		prediction = M;
	else if (max == e_10_count)
		prediction = E10;

	return prediction;
}
void KNN(std::vector<std::tuple<int, int, float, Rating>>& set_A, 
	       std::vector<std::tuple<int, int, float>>& estimating_datas)
{
	for (unsigned i = 0; i < estimating_datas.size(); ++i)
	{
		std::vector<std::tuple<double, Rating>> results;
		std::tuple<int, int, float> curr = estimating_datas[i];
		for (unsigned j = 0; j < set_A.size(); ++j)
		{
			double dist = Dist_Sqrt(std::get<0>(set_A[j]), std::get<1>(set_A[j]),
				std::get<2>(set_A[j]),
				std::get<0>(estimating_datas[i]), std::get<1>(estimating_datas[i]),
				std::get<2>(estimating_datas[i]));
			results.push_back(std::make_tuple(dist, 
				std::get<3>(set_A[j])));
		}
		std::sort(results.begin(), results.end(), [](const std::tuple<double, Rating >& lhs,
			const std::tuple<double, Rating>& rhs)
			{
				return std::get<0>(lhs) < std::get<0>(rhs);
			});
		Rating predict = CheckPrediction(results, 2);
		std::cout << predict << std::endl;
		/*for (int k = 0; k < 6; ++k)
		{
			Rating predict = CheckPrediction(results, k);
		}*/
	}
}
int main(void)
{
	std::vector<std::tuple<int, int, float, Rating>> datas;

	/* Input datas */
	datas.push_back(std::make_tuple(2008, 85, 6.6, T)); // 4
	datas.push_back(std::make_tuple(2008, 87, 4.9, E));
	datas.push_back(std::make_tuple(2008, 81, 7.9, T));
	datas.push_back(std::make_tuple(2008, 56, 5.7, E10));

	datas.push_back(std::make_tuple(2009, 84, 7.3, T)); //8
	datas.push_back(std::make_tuple(2009, 83, 8.4, T));
	datas.push_back(std::make_tuple(2009, 79, 8.1, M));
	datas.push_back(std::make_tuple(2009, 73, 1.8, T));
	datas.push_back(std::make_tuple(2009, 71, 3.0, E10));
	datas.push_back(std::make_tuple(2009, 67, 9.3, E));
	datas.push_back(std::make_tuple(2009, 63, 2.4, E10)); // 13

	datas.push_back(std::make_tuple(2010, 67, 6.6, M));
	datas.push_back(std::make_tuple(2010, 63, 4.2, T));
	datas.push_back(std::make_tuple(2010, 58, 5.5, E));
	datas.push_back(std::make_tuple(2010, 54, 5.5, T));

	datas.push_back(std::make_tuple(2011, 81, 3.9, E)); // 19
	datas.push_back(std::make_tuple(2011, 81, 2.2, T));
	datas.push_back(std::make_tuple(2011, 77, 4.4, T));
	datas.push_back(std::make_tuple(2011, 75, 3.5, E));
	datas.push_back(std::make_tuple(2011, 55, 6, E));
	datas.push_back(std::make_tuple(2011, 49, 6.5, M));
	datas.push_back(std::make_tuple(2011, 48, 5.6, E));

	datas.push_back(std::make_tuple(2012, 70, 3.5, T));
	datas.push_back(std::make_tuple(2012, 56, 6.9, M));
	datas.push_back(std::make_tuple(2012, 50, 6.8, M));
	datas.push_back(std::make_tuple(2012, 47, 6.7, E10));

	datas.push_back(std::make_tuple(2013, 43, 7.9, T)); //30
	datas.push_back(std::make_tuple(2013, 42, 6.3, M));
	datas.push_back(std::make_tuple(2013, 26, 3.4, M));
	datas.push_back(std::make_tuple(2013, 81, 6.2, M));
	datas.push_back(std::make_tuple(2013, 74, 7.2, T));

	datas.push_back(std::make_tuple(2014, 41, 7.7, T));
	datas.push_back(std::make_tuple(2014, 38, 7.9, T));
	datas.push_back(std::make_tuple(2014, 33, 6.6, M));
	datas.push_back(std::make_tuple(2014, 32, 7.1, M));

	datas.push_back(std::make_tuple(2015, 80, 8.9, T));
	datas.push_back(std::make_tuple(2015, 62, 8.6, T));

	datas.push_back(std::make_tuple(2016, 81, 6.2, E));
	datas.push_back(std::make_tuple(2016, 80, 6.7, E));
	datas.push_back(std::make_tuple(2016, 71, 8.7, E));
	datas.push_back(std::make_tuple(2016, 61, 8.3, E));
	datas.push_back(std::make_tuple(2016, 60, 7.1, T));

	std::vector<std::tuple<int, int, float>> datas_test;

	datas_test.push_back(std::make_tuple(2008, 56, 7.3));
	datas_test.push_back(std::make_tuple(2009, 77, 7.5));
	datas_test.push_back(std::make_tuple(2010, 43, 5.6));
	datas_test.push_back(std::make_tuple(2011, 49, 6.2));
	datas_test.push_back(std::make_tuple(2011, 37, 4.5));
	datas_test.push_back(std::make_tuple(2012, 73, 6.2));
	datas_test.push_back(std::make_tuple(2013, 40, 5.8));
	datas_test.push_back(std::make_tuple(2016, 86, 8.2));

	//KNN(datas, datas_test);

	std::vector<std::tuple<int, int, float>> datas_undecided;

	datas_undecided.push_back(std::make_tuple(2009, 85, 5));
	datas_undecided.push_back(std::make_tuple(2013, 56, 8));
	datas_undecided.push_back(std::make_tuple(2015, 18, 2.7));

	KNN(datas, datas_undecided);


	return 0;
}