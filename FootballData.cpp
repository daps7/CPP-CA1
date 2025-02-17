#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

struct FootballData {
    string playerName;
    string team;
    int goals;
    int assists;
    double rating;
};

vector<FootballData> loadCSV(const string &filename) {
    vector<FootballData> data;
    ifstream file(filename);
    string line, word;

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return data;
    }
    getline(file, line);  // Skip header line
    while (getline(file, line)) {
        stringstream ss(line);
        FootballData record;
        getline(ss, record.playerName, ',');
        getline(ss, record.team, ',');
        ss >> record.goals;
        ss.ignore();
        ss >> record.assists;
        ss.ignore();
        ss >> record.rating;
        data.push_back(record);
    }
    file.close();
    return data;
}

void displayData(const vector<FootballData> &data) {
    cout << left << setw(15) << "Player" << setw(15) << "Team"
         << setw(10) << "Goals" << setw(10) << "Assists"
         << setw(10) << "Rating" << endl;
    cout << string(60, '-') << endl;
    for (const auto &record : data) {
        cout << setw(15) << record.playerName << setw(15) << record.team
             << setw(10) << record.goals << setw(10) << record.assists
             << setw(10) << fixed << setprecision(2) << record.rating << endl;
    }
}

int searchByPlayer(const vector<FootballData> &data, const string &playerName) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i].playerName == playerName) {
            return i;
        }
    }
    return -1;
}

map<string, int> countByTeam(const vector<FootballData> &data) {
    map<string, int> teamCounts;
    for (const auto &record : data) {
        teamCounts[record.team]++;
    }
    return teamCounts;
}

void displayByTeam(const vector<FootballData> &data, const string &team) {
    for (const auto &record : data) {
        if (record.team == team) {
            cout << record.playerName << " - Goals: " << record.goals << endl;
        }
    }
}

double calculateStats(const vector<FootballData> &data, FootballData &best, FootballData &worst) {
    if (data.empty()) return 0.0;
    best = worst = data[0];
    double total = 0;
    for (const auto &record : data) {
        total += record.rating;
        if (record.rating > best.rating) best = record;
        if (record.rating < worst.rating) worst = record;
    }
    return total / data.size();
}

vector<FootballData> searchPartialMatch(const vector<FootballData> &data, const string &query) {
    vector<FootballData> results;
    for (const auto &record : data) {
        if (record.playerName.find(query) != string::npos) {
            results.push_back(record);
        }
    }
    return results;
}

void sortDescendingByRating(vector<FootballData> &data) {
    sort(data.begin(), data.end(), [](const FootballData &a, const FootballData &b) {
        return a.rating > b.rating;
    });
}

void menu(const vector<FootballData> &data) {
    int choice;
    string input;
    do {
        cout << "\n1. Display Data\n2. Search by Player\n3. Count by Team\n4. Display by Team";
        cout << "\n5. Get Statistics\n6. Search Partial Match\n7. Sort by Rating\n8. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                displayData(data);
                break;
            case 2: {
                cout << "Enter player name: "; cin >> input;
                int idx = searchByPlayer(data, input);
                if (idx != -1) displayData({data[idx]});
                else cout << "Player not found!" << endl;
                break;
            }
            case 3: {
                auto counts = countByTeam(data);
                for (const auto &[team, count] : counts) {
                    cout << team << ": " << count << " players\n";
                }
                break;
            }
            case 4: {
                cout << "Enter team name: "; cin >> input;
                displayByTeam(data, input);
                break;
            }
            case 5: {
                FootballData best, worst;
                double avg = calculateStats(data, best, worst);
                cout << "Avg Rating: " << avg << " | Best: " << best.playerName << " | Worst: " << worst.playerName << endl;
                break;
            }
            case 6: {
                cout << "Enter partial name: "; cin >> input;
                displayData(searchPartialMatch(data, input));
                break;
            }
            case 7:
                sortDescendingByRating(const_cast<vector<FootballData>&>(data));
                displayData(data);
                break;
            case 8:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 8);
}

int main() {
    vector<FootballData> data = loadCSV("MOCK_DATA.csv");
    if (!data.empty()) menu(data);
    return 0;
}
