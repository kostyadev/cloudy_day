#include <iostream>
#include <fstream> 
#include <vector>
#include <algorithm>
#include <string>
#include <time.h>

using namespace std;

vector<string> split_string(string);

struct City;

struct Cloud
{
	long loc;
	long range;
	Cloud(long l = 0, long r = 0) { loc = l; range = r; }
};

struct City
{
	long loc;
	long pop;
	long cloudCnt = 0;
	City(long l = 0, long p = 0) { loc = l; pop = p; }
	long cloudIncDec = 0;
};

// Complete the maximumPeople function below.
long long maximumPeople(const vector<long>& city_pops, const vector<long>& city_locs, const vector<long>& cloud_locs, const vector<long>& cloud_ranges)
{
	vector<City> cities;
	cities.reserve(city_pops.size());
	for (size_t i = 0; i < city_pops.size(); i++)
	{
		cities.emplace_back(city_locs[i], city_pops[i]);
	}

	// sort cities by location
	sort(cities.begin(), cities.end(), [](const auto& a, const auto& b) { return a.loc < b.loc; });

	vector<Cloud> clouds;
	clouds.reserve(cloud_locs.size());
	for (size_t i = 0; i < cloud_locs.size(); i++)
	{
		clouds.emplace_back(cloud_locs[i], cloud_ranges[i]);
	}

	sort(clouds.begin(), clouds.end(), [](const auto& a, const auto& b) { return a.loc < b.loc; });

	for (Cloud& cloud : clouds)
	{
		// find first city that are covered by this cloud
		auto leftCityIt = lower_bound(cities.begin(), cities.end(), (cloud.loc - cloud.range), [](const auto& item, long val) { return item.loc < val; });
		// find first left city that are NOT covered by this cloud
		auto rightCityIt = upper_bound(cities.begin(), cities.end(), (cloud.loc + cloud.range), [](long val, const auto& item) { return val < item.loc; });

		const auto idxEnd = rightCityIt - cities.begin();
		const auto idxBegin = leftCityIt - cities.begin();
		
		if (idxBegin >= 0 && idxBegin < cities.size())
			cities[idxBegin].cloudIncDec += 1;

		if (idxEnd >= 0 && idxEnd < cities.size())
			cities[idxEnd].cloudIncDec -= 1;
	}

	long curCloudCnt = 0;
	for (City& city : cities)
	{
		curCloudCnt += city.cloudIncDec;
		city.cloudCnt = curCloudCnt;
	}

	// sort cities by covered cloud count
	sort(cities.begin(), cities.end(), [](const auto& a, const auto& b) { return a.cloudCnt < b.cloudCnt; });

	// calculate count of cities that is not covered by any cloud
	long long alreadySunnyPop = 0;
	int lastOneCloudCityIdx = -1;
	for (int i = 0; i < cities.size(); ++i)
	{
		if (cities[i].cloudCnt > 1)
			break;

		if (cities[i].cloudCnt == 0)
			alreadySunnyPop += cities[i].pop;

		lastOneCloudCityIdx = i;
	}

	auto subArrBeg = cities.end();
	auto subArrEnd = cities.end();
	if (lastOneCloudCityIdx >= 0)
	{
		subArrBeg = cities.begin();
		subArrEnd = subArrBeg + lastOneCloudCityIdx + 1;
	}

	// sort by location again, but this time should be faster 
	// because we exclude all cities that are covered more than 1 cloud
	sort(subArrBeg, subArrEnd, [](const auto& a, const auto& b) { return a.loc < b.loc; });
	
	// calculate maximum people that can be sunny by removing one cloud
	long long maxSunnyPop = 0;
	for (const Cloud& cloud : clouds)
	{
		long long curSunnyPop = 0;
		auto leftCityIt = lower_bound(subArrBeg, subArrEnd, (cloud.loc - cloud.range), [](const auto& item, long val) { return item.loc < val; });
		auto rightCityIt = upper_bound(subArrBeg, subArrEnd, (cloud.loc + cloud.range), [](long val, const auto& item) { return val < item.loc; });

		for (auto it = leftCityIt; it != rightCityIt; ++it)
		{
			if (it->cloudCnt == 1)
				curSunnyPop += it->pop;
		}

		if (curSunnyPop > maxSunnyPop)
			maxSunnyPop = curSunnyPop;
	}

	return maxSunnyPop + alreadySunnyPop;
}
/*
int main()
{
	vector<long> city_populations = { 10, 50, 100, 90 };
	vector<long> city_locations = { 5, 7, 9, 15 };
	vector<long> cloud_locations = { 4, 5, 7, 10, 15, 16 };
	vector<long> cloud_ranges = { 1, 2, 1, 1, 1, 1 };
	long result = maximumPeople(city_populations, city_locations, cloud_locations, cloud_ranges);
	cout << endl << result << "\n";


	vector<long> city_populations = { 10, 100 };
	vector<long> city_locations = { 5, 100 };
	vector<long> cloud_locations = { 4 };
	vector<long> cloud_ranges = { 1 };
	long result = maximumPeople(city_populations, city_locations, cloud_locations, cloud_ranges);
	cout << endl << result << "\n";
}
*/

int main()
{
	const auto begin_time = clock();

	ifstream fin("D:\\projects\\hacker_rank\\cloudy_day\\cloudy_day\\input22.txt", std::ofstream::in);

	int n;
	fin >> n;
	fin.ignore(numeric_limits<streamsize>::max(), '\n');

	string p_temp_temp;
	getline(fin, p_temp_temp);

	vector<string> p_temp = split_string(p_temp_temp);

	vector<long> p(n);

	for (int i = 0; i < n; i++) {
		long p_item = stol(p_temp[i]);

		p[i] = p_item;
	}

	string x_temp_temp;
	getline(fin, x_temp_temp);

	vector<string> x_temp = split_string(x_temp_temp);

	vector<long> x(n);

	for (int i = 0; i < n; i++) {
		long x_item = stol(x_temp[i]);

		x[i] = x_item;
	}

	int m;
	fin >> m;
	fin.ignore(numeric_limits<streamsize>::max(), '\n');

	string y_temp_temp;
	getline(fin, y_temp_temp);

	vector<string> y_temp = split_string(y_temp_temp);

	vector<long> y(m);

	for (int i = 0; i < m; i++) {
		long y_item = stol(y_temp[i]);

		y[i] = y_item;
	}

	string r_temp_temp;
	getline(fin, r_temp_temp);

	vector<string> r_temp = split_string(r_temp_temp);

	vector<long> r(m);

	for (int i = 0; i < m; i++) {
		long r_item = stol(r_temp[i]);

		r[i] = r_item;
	}


	std::cout << "read and parse time: " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;

	const auto time2 = clock();

	long long result = maximumPeople(p, x, y, r);

	cout << result << "\n";
	std::cout << "maximumPeople() time: " << float(clock() - time2) / CLOCKS_PER_SEC << endl;

	return 0;
}

vector<string> split_string(string input_string) {
	string::iterator new_end = unique(input_string.begin(), input_string.end(), [](const char &x, const char &y) {
		return x == y and x == ' ';
	});

	input_string.erase(new_end, input_string.end());

	while (input_string[input_string.length() - 1] == ' ') {
		input_string.pop_back();
	}

	vector<string> splits;
	char delimiter = ' ';

	size_t i = 0;
	size_t pos = input_string.find(delimiter);

	while (pos != string::npos) {
		splits.push_back(input_string.substr(i, pos - i));

		i = pos + 1;
		pos = input_string.find(delimiter, i);
	}

	splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

	return splits;
}