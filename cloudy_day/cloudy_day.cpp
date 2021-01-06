#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct City;

struct Cloud
{
	long loc;
	long range;
	Cloud(long l = 0, long r = 0) { loc = l; range = r; }
	vector<const City*> cities;
};

struct City
{
	long loc;
	long pop;
	City(long l = 0, long p = 0) { loc = l; pop = p; }
	vector<const Cloud*> clouds;
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

	cout << "cities:" << endl;
	for (auto& c : cities)
		cout << c.loc << " " << c.pop << endl;

	cout << "clouds:" << endl;
	for (auto& c : clouds)
		cout << c.loc << " " << c.range << endl;

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

	cout << endl;
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
	}

	long maxSunnyPop = 0;
	for (const Cloud& cloud : clouds)
	{
		long curSunnyPop = 0;
		for (const City* city : cloud.cities)
		{ 
			if (city->clouds.size() == 1 && city->clouds[0] == &cloud)
				curSunnyPop += city->pop;
		}

		if (curSunnyPop > maxSunnyPop)
			maxSunnyPop = curSunnyPop;
	}

	return maxSunnyPop;
}

int main()
{
	vector<long> city_populations = { 10, 50, 100, 90 };
	vector<long> city_locations = { 5, 7, 9, 15 };
	vector<long> cloud_locations = { 4, 5, 7, 10, 15, 16 };
	vector<long> cloud_ranges = { 1, 2, 1, 1, 1, 1 };
	long result = maximumPeople(city_populations, city_locations, cloud_locations, cloud_ranges);
	cout << endl << result << "\n";
}
