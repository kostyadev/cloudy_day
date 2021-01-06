#include <iostream>
#include <fstream> 
#include <vector>
#include <list>
#include <algorithm>
#include <string>

using namespace std;

vector<string> split_string(string);

struct City;

struct Cloud
{
	long loc;
	long range;
	Cloud(long l = 0, long r = 0) { loc = l; range = r; }
	list<const City*> cities;
};

struct City
{
	long loc;
	long pop;
	City(long l = 0, long p = 0) { loc = l; pop = p; }
	list<const Cloud*> clouds;
};

// binary search in sorted array
size_t getNearestItemPos(const long pos, const vector<City>& cities)
{
	auto it = lower_bound(cities.begin(), cities.end(), pos, [](const auto& item, long val) { return item.loc < val; });
	if (it == cities.end())
	{
		return cities.size() - 1;
	}

	return distance(cities.begin(), it);
}

// Complete the maximumPeople function below.
long long maximumPeople(const vector<long>& city_pops, const vector<long>& city_locs, const vector<long>& cloud_locs, const vector<long>& cloud_ranges)
{
	vector<City> cities;
	cities.reserve(city_pops.size());
	for (size_t i = 0; i < city_pops.size(); i++)
	{
		cities.emplace_back(city_locs[i], city_pops[i]);
	}

	sort(cities.begin(), cities.end(), [](const auto& a, const auto& b) { return a.loc < b.loc; });

	vector<Cloud> clouds;
	clouds.reserve(cloud_locs.size());
	for (size_t i = 0; i < cloud_locs.size(); i++)
	{
		clouds.emplace_back(cloud_locs[i], cloud_ranges[i]);
	}

	sort(clouds.begin(), clouds.end(), [](const auto& a, const auto& b) { return a.loc < b.loc; });

	/*cout << "cities:" << endl;
	for (auto& c : cities)
		cout << c.loc << " " << c.pop << endl;

	cout << "clouds:" << endl;
	for (auto& c : clouds)
		cout << c.loc << " " << c.range << endl;*/

	for (auto& cloud : clouds)
	{
		const auto cityIdx = getNearestItemPos(cloud.loc, cities);

		//search all cities that overriden by this cloud on right side
		auto rCityIdx = cityIdx;
		while (rCityIdx < cities.size() && cities[rCityIdx].loc <= (cloud.loc + cloud.range))
		{
			cities[rCityIdx].clouds.push_back(&cloud);
			cloud.cities.push_back(&cities[rCityIdx]);
			rCityIdx++;
		}

		//search on left side
		auto lCityIdx = cityIdx - 1;
		while (lCityIdx >= 0 && lCityIdx < cities.size() && cities[lCityIdx].loc >= (cloud.loc - cloud.range))
		{
			cities[lCityIdx].clouds.push_back(&cloud);
			cloud.cities.push_back(&cities[lCityIdx]);
			lCityIdx--;
		}
	}

	/*cout << endl;
	for (size_t i = 0; i < cities.size(); ++i)
	{
		cout << "cities[" << i << "], {loc " << cities[i].loc << ", pop " << cities[i].pop << "}, clouds [";
		for (const auto& cloud : cities[i].clouds)
			cout << "{loc " << cloud->loc << ", range " << cloud->range << "}";
		cout << "]" << endl;
	}

	cout << endl;
	for (size_t i = 0; i < clouds.size(); ++i)
	{
		cout << "clouds[" << i << "], {loc " << clouds[i].loc << ", range " << clouds[i].range << "}, cities [";
		for (const auto& city : clouds[i].cities)
			cout << "{loc " << city->loc << ", pop " << city->pop << "}";
		cout << "]" << endl;
	}*/

	// calculate count of cities that is not covered by any cloud
	long long alreadySunnyPop = 0;
	long alreadySunnyPopPrevVal = 0;
	for (const City& city : cities)
	{
		if (city.clouds.size() == 0)
			alreadySunnyPop += city.pop;

		if (alreadySunnyPop < 0)
			int a = 1;

		alreadySunnyPopPrevVal = alreadySunnyPop;
	}

	// calculate maximum people that can be sunny by removing one cloud
	long maxSunnyPop = 0;
	for (const Cloud& cloud : clouds)
	{
		long curSunnyPop = 0;
		for (const City* city : cloud.cities)
		{ 
			if (city->clouds.size() == 1 && *(city->clouds.begin()) == &cloud)
				curSunnyPop += city->pop;
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
	ifstream fin("D:\\projects\\hacker_rank\\cloudy_day\\cloudy_day\\input26.txt", std::ofstream::in);

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

	long long result = maximumPeople(p, x, y, r);

	cout << result << "\n";

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