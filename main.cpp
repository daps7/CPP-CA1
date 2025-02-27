#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Player //player struct
{
    int playerID;
    string firstName;
    string lastName;
    int goals;
    int matchesPlayed;
};

vector<Player> loadCSV(const string &filename) {
    vector<Player> data;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return data;
    }
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        Player record;
        string temp;
        getline(ss, temp, ',');
        record.playerID = stoi(temp);
        getline(ss, record.firstName, ',');
        getline(ss, record.lastName, ',');
        getline(ss, temp, ',');
        record.goals = stoi(temp);
        getline(ss, temp, ',');
        record.matchesPlayed = stoi(temp);
        data.push_back(record);
    }
    file.close();
    return data;
}
//displaying data
void displayData(const vector<Player> &data) {
    cout << left << setw(10) << "ID" << setw(15) << "First Name" << setw(15) << "Last Name"
         << setw(10) << "Goals" << setw(15) << "Matches Played" << endl;
    cout << string(60, '-') << endl;
    for (const auto &record : data) {
        cout << setw(10) << record.playerID << setw(15) << record.firstName << setw(15) << record.lastName
             << setw(10) << record.goals << setw(15) << record.matchesPlayed << endl;
    }
}

int searchByPlayerID(const vector<Player> &data, int playerID) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i].playerID == playerID) {
            return i;
        }
    }
    return -1;
}

map<string, int> countByLastName(const vector<Player> &data) {
    map<string, int> lastNameCounts;
    for (const auto &record : data) {
        lastNameCounts[record.lastName]++;
    }
    return lastNameCounts;
}

void displayByLastName(const vector<Player> &data, const string &lastName) {
    for (const auto &record : data) {
        if (record.lastName == lastName) {
            cout << record.firstName << " " << record.lastName << " - Goals: " << record.goals << endl;
        }
    }
}

pair<Player, Player> findMinMaxGoals(const vector<Player> &data, double &average) {
    if (data.empty()) return {};
    Player most = data[0], lowest = data[0];
    int totalGoals = 0;
    for (const auto &record : data) {
        totalGoals += record.goals;
        if (record.goals > most.goals) most = record;
        if (record.goals < lowest.goals) lowest = record;
    }
    average = static_cast<double>(totalGoals) / data.size();
    return {most, lowest};
}

vector<Player> searchPartialMatch(const vector<Player> &data, const string &query) {
    vector<Player> results;
    for (const auto &record : data) {
        if (record.firstName.find(query) != string::npos || record.lastName.find(query) != string::npos) {
            results.push_back(record);
        }
    }
    return results;
}

void sortDescendingByMatchesPlayed(vector<Player> &data) {
    sort(data.begin(), data.end(), [](const Player &a, const Player &b) {
        return a.matchesPlayed > b.matchesPlayed;
    });
}

void menu(vector<Player> &data) {
    int choice, playerID;
    string input;
    cout <<"*********************************************\n*******Welcome To My Football Data Menu******\n*********************************************"<< endl;
    do {
        cout << "\n1. Display Data\n2. Search by Player ID\n3. Count by Last Name\n4. Display by Last Name";
        cout << "\n5. Get Min/Max/Average Goals\n6. Search Partial Match\n7. Sort by Matches Played\n8. Exit\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayData(data);
                break;
            case 2: {
                cout << "Enter Player ID: ";
                cin >> playerID;
                int player_id = searchByPlayerID(data, playerID); // Move inside a block {}
                if (player_id != -1) displayData({data[player_id]});
                else cout << "Player not found!" << endl;
                break;
            }
            case 3: {
                auto counts = countByLastName(data);
                cout<< "Choose the last name you want to search"<<endl;
                string input_lname;
                cin>>input_lname;
                for (const auto &[lastName, count] : counts) {
                    if(input_lname.find(lastName) != string::npos) {
                    cout << lastName << ": " << count << " players\n";
                    }
                }
                break;
            }
            case 4:
                cout << "Enter Last Name: ";
                cin >> input;
                displayByLastName(data, input);
                break;
            case 5: {
                double avg;
                auto [best, worst] = findMinMaxGoals(data, avg);
                cout << "Avg Goals: " << avg << " | Best: " << best.firstName << " " << best.lastName
                     << " | Worst: " << worst.firstName << " " << worst.lastName << endl;
                break;
            }
            case 6:
                cout << "Enter partial name: ";
                cin >> input;
                displayData(searchPartialMatch(data, input));
                break;
            case 7:
                sortDescendingByMatchesPlayed(data);
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
    vector<Player> data = loadCSV("MOCK_DATA.csv");
    if (!data.empty()) menu(data);
    return 0;
}
