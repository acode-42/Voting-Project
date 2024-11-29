#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <algorithm> 

using namespace std;

class VotingSystem {
private:
    map<string, int> candidates;
    vector<string> voterIds;

    void saveData() {
        ofstream file("voting_data.txt");
        if (file.is_open()) {
            file << candidates.size() << "\n";
            for (const auto& candidate : candidates) {
                file << candidate.first << " " << candidate.second << "\n";
            }

            file << voterIds.size() << "\n";
            for (const auto& voterId : voterIds) {
                file << voterId << "\n";
            }

            file.close();
        } else {
            cout << "Error: Unable to save data.\n";
        }
    }

    void loadData() {
        ifstream file("voting_data.txt");
        if (file.is_open()) {
            size_t candidatesCount;
            file >> candidatesCount;
            candidates.clear();
            for (size_t i = 0; i < candidatesCount; ++i) {
                string candidateName;
                int votes;
                file >> candidateName >> votes;
                candidates[candidateName] = votes;
            }

            size_t votersCount;
            file >> votersCount;
            voterIds.clear();
            for (size_t i = 0; i < votersCount; ++i) {
                string voterId;
                file >> voterId;
                voterIds.push_back(voterId);
            }

            file.close();
        } else {
            cout << "No existing data found. Starting fresh.\n";
        }
    }

public:
    VotingSystem() {
        loadData();
    }

    ~VotingSystem() {
        saveData();
    }

    void addCandidate(const string& candidateName) {
        if (candidateName.empty() || candidateName.find(' ') != string::npos) {
            cout << "Invalid candidate name. It should be a non-empty string without spaces.\n";
            return;
        }

        if (candidates.find(candidateName) == candidates.end()) {
            candidates[candidateName] = 0;
            cout << "Candidate " << candidateName << " added successfully.\n";
        } else {
            cout << "Candidate " << candidateName << " is already in the system.\n";
        }
    }

    void castVote(const string& voterId, const string& candidateName) {
        if (voterId.empty() || candidateName.empty()) {
            cout << "Invalid input. Voter ID and candidate name cannot be empty.\n";
            return;
        }

        if (std::find(voterIds.begin(), voterIds.end(), voterId) != voterIds.end()) {
            cout << "Voter with ID " << voterId << " has already voted.\n";
            return;
        }

        if (candidates.find(candidateName) != candidates.end()) {
            candidates[candidateName]++;
            voterIds.push_back(voterId);
            cout << "Vote cast successfully for " << candidateName << ".\n";
        } else {
            cout << "Candidate " << candidateName << " not found.\n";
        }
    }

    void displayResults() const {
        cout << "\nVoting Results:\n";
        for (const auto& candidate : candidates) {
            cout << candidate.first << ": " << candidate.second << " votes\n";
        }
    }

    void declareWinner() const {
        string winner;
        int maxVotes = 0;
        bool tie = false;

        for (const auto& candidate : candidates) {
            if (candidate.second > maxVotes) {
                winner = candidate.first;
                maxVotes = candidate.second;
                tie = false;
            } else if (candidate.second == maxVotes) {
                tie = true;
            }
        }

        if (tie || maxVotes == 0) {
            cout << "\nThe election resulted in a tie or no votes were cast.\n";
        } else {
            cout << "\nThe winner is: " << winner << " with " << maxVotes << " votes!\n";
        }
    }
};

int main() {
    VotingSystem votingSystem;
    int choice;

    do {
        cout << "\nVoting System Menu:\n";
        cout << "1. Add Candidate\n";
        cout << "2. Cast Vote\n";
        cout << "3. Display Results\n";
        cout << "4. Declare Winner\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            string candidateName;
            cout << "Enter candidate name: ";
            cin >> candidateName;
            votingSystem.addCandidate(candidateName);
            break;
        }
        case 2: {
            string voterId, candidateName;
            cout << "Enter voter ID: ";
            cin >> voterId;
            cout << "Enter candidate name: ";
            cin >> candidateName;
            votingSystem.castVote(voterId, candidateName);
            break;
        }
        case 3:
            votingSystem.displayResults();
            break;
        case 4:
            votingSystem.declareWinner();
            break;
        case 5:
            cout << "Exiting the system.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
