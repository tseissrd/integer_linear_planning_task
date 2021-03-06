#include <pch.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <limits>
using namespace std;
const int inf = numeric_limits<int>::max();
const double epsilon = 0.0000001;
class itemtype
{
public:
	vector<double> value;
	double price;
	bool mu, isint;
};
class simplextable
{
public:
	vector<itemtype> item;
	vector<double> goal;
	double cost;
	int itemcount, values;
	bool final;
};
void help()
{
	cout << "(integer) linear planning task\ni - create a new item\ng - set the value goals\nd - display the current setup\nc - calculate the result\nr - reset everything\n\n1 - load 1.49 setup\nh - display this message\ne - exit\n\n";
}
void pause()
{
	string nothing;
	cout << "Press ENTER to exit";
	getchar();
	getline(cin, nothing);
}
double frac(double a)
{
	double c;
	if (a < 0)
		c = int(a) - 1;
	if (a >= 0)
		c = int(a);
	return a - c;
}
int main()
{
	int i, j, k, r, rows, columns;
	int choicer, choicec, nulls, splitx, splitr, lastr;
	vector<double> b, b0, c, dj, theta;
	vector<double> djm;
	vector<vector<double>> a, a0;
	vector<int> base;
	vector<simplextable> table;
	string input;
	double cost, splitg;
	bool min, opt, check, mcheck, zcheck(0), goalsset, valuesset, artif;
	help();
snvalues:
	table.reserve(1);
	table.resize(1);
	valuesset = 0;
	goalsset = 0;
	r = 0;
	lastr = 0;
	table[r].itemcount = 0;
	min = 0;
	cout << "Number of values per item:";
	cin >> table[r].values;
	table[r].goal.reserve(table[r].values);
	table[r].goal.resize(table[r].values);
newopt:
	cin >> input;
	if (input == "i")
	{
		table[r].item.reserve(table[r].itemcount + 1);
		table[r].item.resize(table[r].itemcount + 1);
		table[r].item[table[r].itemcount].value.reserve(table[r].values);
		table[r].item[table[r].itemcount].value.resize(table[r].values);
		for (i = 0; i < table[r].values; ++i)
		{
			cout << "Value " << i + 1 << ":";
			cin >> table[r].item[table[r].itemcount].value[i];
		}
		cout << "Price:";
		cin >> input;
		if (input == "-mu" || input == "mu")
		{
			table[r].item[table[r].itemcount].price = 1;
			table[r].item[table[r].itemcount].mu = 1;
		}
		else
		{
			table[r].item[table[r].itemcount].price = stod(input);
			table[r].item[table[r].itemcount].mu = 0;
		}
		if (min && !table[r].item[table[r].itemcount].mu)
			table[r].item[table[r].itemcount].price *= -1;
	iqt:
		cout << "Of integer quantity(y/n):";
		cin >> input;
		if (input == "y")
			table[r].item[table[r].itemcount].isint = 1;
		else
		{
			if (input == "n")
				table[r].item[table[r].itemcount].isint = 0;
			else
				goto iqt;
		}
		++table[r].itemcount;
		valuesset = 1;
	}
	if (input == "d" && valuesset && goalsset)
	{
	display:
		cout << "-----------------------\nTable " << r + 1 << "/" << lastr + 1 << endl << "-----------------------" << endl;
		cout << "F=";
		for (i = 0; i < table[r].itemcount; ++i)
		{
			if (table[r].item[i].price != 0)
			{
				k = i + 1;
				if (!table[r].item[i].mu)
					cout << table[r].item[i].price << "x" << i + 1 << " ";
				else
					cout << "-mux" << i + 1 << " ";
				goto cprices;
			}
		}
		goto newopt;
	cprices:
		for (i = k; i < table[r].itemcount; ++i)
		{
			if (!table[r].item[i].mu)
			{
				if (table[r].item[i].price > 0)
					cout << "+ " << table[r].item[i].price << "x" << i + 1 << " ";
				if (table[r].item[i].price < 0)
					cout << "- " << -table[r].item[i].price << "x" << i + 1 << " ";
			}
			else
			{
				cout << "- mux" << i + 1 << " ";
			}
		}
		cout << "-> max";
		cout << "\n\n";
		for (i = 0; i < table[r].values; ++i)
		{
			for (j = 0; j < table[r].itemcount; ++j)
			{
				if (table[r].item[j].value[i] > 0)
					cout << " ";
				if (table[r].item[j].value[i] != 0)
				{
					k = j + 1;
					cout << table[r].item[j].value[i] << "x" << j + 1 << " ";
					goto cvalues;
				}
			}
		cvalues:
			for (j = k; j < table[r].itemcount; ++j)
			{
				if (table[r].item[j].value[i] > 0)
					cout << "+ " << table[r].item[j].value[i] << "x" << j + 1 << " ";
				if (table[r].item[j].value[i] < 0)
					cout << "- " << -table[r].item[j].value[i] << "x" << j + 1 << " ";
			}
			cout << "= " << table[r].goal[i] << endl;
		}
		if (zcheck)
		{
			zcheck = 0;
			goto cnextr;
		}
	}
	if (input == "1")
	{
		table[r].itemcount = 6;
		table[r].item.reserve(table[r].itemcount);
		table[r].item.resize(table[r].itemcount);
		for (i = 0; i < 6; ++i)
		{
			table[r].item[i].value.reserve(table[r].values);
			table[r].item[i].value.resize(table[r].values);
		}
		//x1
		table[r].item[0].value[0] = 2;
		table[r].item[0].value[1] = -3;
		table[r].item[0].value[2] = 1;
		table[r].item[0].price = 3;
		//x2
		table[r].item[1].value[0] = 1;
		table[r].item[1].value[1] = 0;
		table[r].item[1].value[2] = 0;
		table[r].item[1].price = 0;
		//x3
		table[r].item[2].value[0] = -3;
		table[r].item[2].value[1] = 2;
		table[r].item[2].value[2] = 3;
		table[r].item[2].price = 2;
		//x4
		table[r].item[3].value[0] = 0;
		table[r].item[3].value[1] = 1;
		table[r].item[3].value[2] = 0;
		table[r].item[3].price = 0;
		//x5
		table[r].item[4].value[0] = 0;
		table[r].item[4].value[1] = 0;
		table[r].item[4].value[2] = 1;
		table[r].item[4].price = 0;
		//x6
		table[r].item[5].value[0] = 6;
		table[r].item[5].value[1] = -2;
		table[r].item[5].value[2] = -4;
		table[r].item[5].price = -6;
		//b
		table[r].goal[0] = 18;
		table[r].goal[1] = 24;
		table[r].goal[2] = 36;

		valuesset = 1;
		goalsset = 1;
	}
	if (input == "2")
	{
		table[r].itemcount = 4;
		table[r].item.reserve(table[r].itemcount);
		table[r].item.resize(table[r].itemcount);
		for (i = 0; i < table[r].itemcount; ++i)
		{
			table[r].item[i].value.reserve(table[r].values);
			table[r].item[i].value.resize(table[r].values);
		}
		//x1
		table[r].item[0].value[0] = 21;
		table[r].item[0].value[1] = 12;
		table[r].item[0].price = 3;
		table[r].item[0].isint = 1;
		//x2
		table[r].item[1].value[0] = -9;
		table[r].item[1].value[1] = 3;
		table[r].item[1].price = 4;
		table[r].item[1].isint = 1;
		//x3
		table[r].item[2].value[0] = -1;
		table[r].item[2].value[1] = 0;
		table[r].item[2].price = 0;
		//x4
		table[r].item[3].value[0] = 0;
		table[r].item[3].value[1] = 1;
		table[r].item[3].price = 0;
		//b
		table[r].goal[0] = 13;
		table[r].goal[1] = 40;

		valuesset = 1;
		goalsset = 1;
	}
	if (input == "3")
	{
		table[r].itemcount = 5;
		table[r].item.reserve(table[r].itemcount);
		table[r].item.resize(table[r].itemcount);
		for (i = 0; i < 5; ++i)
		{
			table[r].item[i].value.reserve(table[r].values);
			table[r].item[i].value.resize(table[r].values);
		}
		//x1
		table[r].item[0].value[0] = 1;
		table[r].item[0].value[1] = 1;
		table[r].item[0].value[2] = -3;
		table[r].item[0].price = 3;
		table[r].item[0].isint = 1;
		//x2
		table[r].item[1].value[0] = 1;
		table[r].item[1].value[1] = -1;
		table[r].item[1].value[2] = 1;
		table[r].item[1].price = 2;
		table[r].item[1].isint = 1;
		//x3
		table[r].item[2].value[0] = 1;
		table[r].item[2].value[1] = 0;
		table[r].item[2].value[2] = 0;
		table[r].item[2].price = 0;
		table[r].item[2].isint = 1;
		//x4
		table[r].item[3].value[0] = 0;
		table[r].item[3].value[1] = 1;
		table[r].item[3].value[2] = 0;
		table[r].item[3].price = 0;
		table[r].item[3].isint = 1;
		//x5
		table[r].item[4].value[0] = 0;
		table[r].item[4].value[1] = 0;
		table[r].item[4].value[2] = 1;
		table[r].item[4].price = 0;
		table[r].item[4].isint = 1;
		//b
		table[r].goal[0] = 13;
		table[r].goal[1] = 6;
		table[r].goal[2] = 9;

		valuesset = 1;
		goalsset = 1;
	}
	if (input == "g")
	{
		for (i = 0; i < table[r].values; ++i)
		{
		newg:
			cout << "Goal for value " << i + 1 << ":";
			cin >> table[r].goal[i];
			if (table[r].goal[i] < 0)
				goto newg;
		}
		goalsset = 1;
	}
	if (input == "c" && valuesset && goalsset)
	{
	cnextr:
		rows = table[r].values;
		columns = table[r].itemcount;
		base.reserve(rows);
		base.resize(rows);
		c.reserve(columns);
		c.resize(columns);
		a.reserve(rows);
		a.resize(rows);
		b.reserve(rows);
		b.resize(rows);
		dj.reserve(columns);
		dj.resize(columns);
		djm.reserve(columns);
		djm.resize(columns);
		theta.reserve(rows);
		theta.resize(rows);
		for (i = 0; i < rows; ++i)
		{
			a[i].reserve(columns);
			a[i].resize(columns);
		}
		for (i = 0; i < rows; ++i)														//
		{
			for (j = 0; j < table[r].itemcount; ++j)
			{
				c[j] = table[r].item[j].price;
				a[i][j] = table[r].item[j].value[i];
			}
			b[i] = table[r].goal[i];
		}																	//
		for (i = 0; i < rows; ++i)
		{
			base[i] = -1;
			for (j = 0; j < columns; ++j)
			{
				if (a[i][j] == 1)
				{
					check = 1;
					for (k = 0; k < rows; ++k)
						if (a[k][j] != 0 && k != i)
							check = 0;
					if (check)
					{
						base[i] = j;
					}
				}
			}
		}
		for (i = 0; i < rows; ++i)
			if (base[i] == -1)
			{
				++table[r].itemcount;
				cout << "NEW BASE ADDED (x" << table[r].itemcount << ")" << endl;
				table[r].item.reserve(table[r].itemcount);
				table[r].item.resize(table[r].itemcount);
				table[r].item[table[r].itemcount - 1].value.reserve(table[r].values);
				table[r].item[table[r].itemcount - 1].value.resize(table[r].values);
				columns = table[r].itemcount;
				c.reserve(columns);
				c.resize(columns);
				dj.reserve(columns);
				dj.resize(columns);
				djm.reserve(columns);
				djm.resize(columns);
				for (k = 0; k < rows; ++k)
				{
					a[k].reserve(columns);
					a[k].resize(columns);
				}
				base[i] = table[r].itemcount - 1;
				table[r].item[table[r].itemcount - 1].value[i] = 1;
				a[i][base[i]] = 1;
				for (j = 0; j < rows; ++j)
					if (j != i)
					{
						table[r].item[table[r].itemcount - 1].value[j] = 0;
						a[j][base[i]] = 0;
					}
				table[r].item[table[r].itemcount - 1].price = 1;
				table[r].item[table[r].itemcount - 1].mu = 1;
				table[r].item[table[r].itemcount - 1].isint = 0;
				zcheck = 1;
				goto display;
			}
	newcheck:
		choicec = 0;
		choicer = 0;
		opt = 1;
		mcheck = 0;
		for (i = 0; i < columns; ++i)
		{
			dj[i] = 0;
			djm[i] = 0;
			for (j = 0; j < rows; ++j)
			{
				if (!table[r].item[base[j]].mu)
					dj[i] += a[j][i] * c[base[j]];
				else djm[i] -= a[j][i];
			}
			if (!table[r].item[i].mu)
				dj[i] -= c[i];
			if (table[r].item[i].mu)
				++djm[i];
			if (djm[i] < 0)
				mcheck = 1;
			if (!mcheck)
			{
				if (dj[i] < dj[choicec])
				{
					choicec = i;
				}
			}
			else
			{
				if (djm[i] < djm[choicec])
				{
					choicec = i;
				}
				if (djm[i] == djm[choicec])
				{
					if (dj[i] < dj[choicec])
					{
						choicec = i;
					}
				}
			}
			if ((djm[i] <= 0 && dj[i] < 0) || djm[i] < 0)
				opt = 0;
		}
		if (opt)
			goto done;
		for (i = 0; i < columns; ++i)
		{
			if (dj[i] < 0 || djm[i] < 0)
			{
				check = 0;
				for (j = 0; j < rows; ++j)
					if (a[j][i] > 0)
						check = 1;
				if (!check)
				{
					cout << "No possible solution\n";
					goto newopt;
				}
			}
		}
		for (i = 0; i < rows; i++)
		{
			if (a[i][choicec] <= 0)
				theta[i] = inf;
			else
				theta[i] = b[i] / a[i][choicec];
			cout << "theta[" << i + 1 << "] = " << theta[i] << endl;
			if (theta[i] < theta[choicer])
				choicer = i;
		}
		artif = 0;
		if (table[r].item[base[choicer]].mu)
		{
			artif = 1;
			nulls = base[choicer];
		}
		a0 = a;
		b0 = b;
		for (i = 0; i < rows; ++i)
		{
			a[i][choicec] = 0;
		}
		a[choicer][choicec] = 1;
		b[choicer] = b0[choicer] / a0[choicer][choicec];
		for (i = 0; i < columns; ++i)
		{
			a[choicer][i] = a0[choicer][i] / a0[choicer][choicec];
		}
		base[choicer] = choicec;
		for (i = 0; i < rows; ++i)
		{
			if (i != choicer)
			{
				b[i] = ((b0[i] * a0[choicer][choicec]) - (b0[choicer] * a0[i][choicec])) / a0[choicer][choicec];
				for (j = 0; j < columns; ++j)
				{
					a[i][j] = ((a0[i][j] * a0[choicer][choicec]) - (a0[choicer][j] * a0[i][choicec])) / a0[choicer][choicec];
				}
			}
		}
		if (artif)
		{
			if (table[r].itemcount > nulls + 1)
			{
				for (i = 0; i < rows; ++i)
				{
					a[i][nulls] = a[i][table[r].itemcount - 1];
					if (base[i] == (table[r].itemcount - 1))
						base[i] = nulls;
				}
				c[nulls] = c[table[r].itemcount - 1];
				table[r].item[nulls] = table[r].item[table[r].itemcount - 1];
				nulls = table[r].itemcount - 1;
			}
			for (i = 0; i < rows; ++i)
				a[i][nulls] = 0;
			c[nulls] = 0;
			table[r].item[nulls].price = 0;
			table[r].item[nulls].mu = 0;
			--table[r].itemcount;
			table[r].item.resize(table[r].itemcount);
			columns = table[r].itemcount;
			c.resize(columns);
			dj.resize(columns);
			djm.resize(columns);
			for (k = 0; k < rows; ++k)
			{
				a[k].resize(columns);
			}
		}
		for (i = 0; i < rows; ++i)
		{
			cout << "b[" << i + 1 << "] = " << b[i] << endl;
			for (j = 0; j < columns; ++j)
			{
				cout << "a[" << i + 1 << "][" << j + 1 << "] = " << a[i][j] << endl;
			}
		}
		goto newcheck;
	}
	if (input == "r")
	{
		goto snvalues;
	}
	if (input == "h")
		help();
	if (input == "e")
		goto exito;
	goto newopt;
done:
	cost = 0;
	for (i = 0; i < rows; ++i)
	{
		cout << "x" << base[i] + 1 << " = " << b[i] << endl;
		if (!table[r].item[base[i]].mu)
			cost += b[i] * c[base[i]];
	}
	if (min)
		cout << "F = " << -cost << endl;
	else
		cout << "F = " << cost << endl;
	for (i = 0; i < rows; ++i)
		if (table[r].item[base[i]].isint && ((fabs(float(long(b[i])) - float(b[i])) > epsilon) && (fabs((float(long(b[i])) + 1) - float(b[i])) > epsilon)))
		{
			splitx = base[i];
			splitr = i;
			splitg = b[i];
			goto split;
		}
	table[r].final = 1;
	table[r].cost = cost;
nextr:
	if (r < lastr)
	{
		++r;
		zcheck = 1;
		goto display;
	}
	if (r == lastr)
		goto finalize;
split:
	cout << "-----------------------\nConstraining with x" << splitx + 1 << endl;
	table.reserve(table.size() + 1);
	table.resize(table.size() + 1);
	table[lastr + 1] = table[r];
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < columns; ++j)
		{
			table[lastr + 1].item[j].value[i] = a[i][j];
		}
		table[lastr + 1].goal[i] = b[i];
	}
	++table[lastr + 1].values;
	table[lastr + 1].goal.reserve(table[lastr + 1].goal.size() + 1);
	table[lastr + 1].goal.resize(table[lastr + 1].goal.size() + 1);
	table[lastr + 1].goal[table[lastr + 1].values - 1] = frac(splitg);
	++table[lastr + 1].itemcount;
	table[lastr + 1].item.reserve(table[lastr + 1].item.size() + 1);
	table[lastr + 1].item.resize(table[lastr + 1].item.size() + 1);
	for (i = 0; i < table[lastr + 1].itemcount; ++i)
	{
		table[lastr + 1].item[i].value.reserve(table[lastr + 1].values);
		table[lastr + 1].item[i].value.resize(table[lastr + 1].values);
	}
	for (i = 0; i < table[lastr + 1].values; ++i)
		table[lastr + 1].item[table[lastr + 1].itemcount - 1].value[i] = 0;
	table[lastr + 1].item[table[lastr + 1].itemcount - 1].value[table[lastr + 1].values - 1] = -1;
	for (i = 0; i < columns; ++i)
	{
		table[lastr + 1].item[i].value[table[lastr].values] = frac(a[splitr][i]);
	}
	++lastr;
	++r;
	zcheck = 1;
	goto display;
finalize:
	goto newopt;
exito:
	pause();
	return 0;
}