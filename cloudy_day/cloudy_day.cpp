#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct City
{
	long loc;
	long pop;
	City(long l = 0, long p = 0) { loc = l; pop = p; }
};

struct Cloud
{
	long loc;
	long range;
	Cloud(long l = 0, long r = 0) { loc = l; range = r; }
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
long maximumPeople(const vector<long>& city_pops, const vector<long>& city_locs, const vector<long>& cloud_locs, const vector<long>& cloud_ranges)
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

	for (auto& c : cities)
		cout << c.loc << " " << c.pop << endl;

	for (auto& c : clouds)
		cout << c.loc << " " << c.range << endl;

	//
	auto p = getNearestItemPos(8, cities);

	return 0;
}

int main()
{
	vector<long> city_populations = { 10, 50, 100, 100 };
	vector<long> city_locations = { 5, 7, 9, 15 };
	vector<long> cloud_locations = { 4, 7, 10 };
	vector<long> cloud_ranges = { 1, 1, 1 };
	long result = maximumPeople(city_populations, city_locations, cloud_locations, cloud_ranges);
	cout << endl << result << "\n";
}
