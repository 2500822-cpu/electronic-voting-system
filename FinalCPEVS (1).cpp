#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const int MAX_CANDIDATES = 5;
const int MAX_VOTERS = 75;
const string ADMIN_USER = "admin";
const string ADMIN_PASS = "password";
const int MIN_AGE = 18;

struct Candidate {
    string id, name, party, symbol;
};

struct Voter {
    string id, name, password;
    int age = 0;
    bool hasVoted = false;
    int votedFor = -1;
};

Candidate candidates[MAX_CANDIDATES];
Voter voters[MAX_VOTERS];
int votes[MAX_CANDIDATES] = {0};
int candidateCount = 0, voterCount = 0;
bool votingOngoing = false;

void Clear() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

bool IsNumeric(const string &s) {
    for (char c : s) 
    if (!isdigit(c)) 
    return false;
    return !s.empty();
}

bool HumanVerify() {
    int pin = rand() % 9000 + 1000, input;
    cout << "\nHuman Verification - Enter PIN: " << pin << "\nPIN: ";
    cin >> input;
    return input == pin;
}

void SaveCandidates() {
    ofstream f("candidates.txt");
    f << candidateCount << '\n';
    for (int i = 0; i < candidateCount; i++)
        f << candidates[i].id << '\n' << candidates[i].name << '\n' 
          << candidates[i].party << '\n' << candidates[i].symbol << '\n' << votes[i] << '\n';
}

void SaveVoters() {
    ofstream f("voters.txt");
    f << voterCount << '\n';
    for (int i = 0; i < voterCount; i++)
        f << voters[i].id << '\n' << voters[i].name << '\n' << voters[i].password << '\n'
          << voters[i].age << '\n' << voters[i].hasVoted << '\n' << voters[i].votedFor << '\n';
}

void LoadCandidates() {
    ifstream f("candidates.txt");
    if (!f) return;
    f >> candidateCount;
    f.ignore();
    for (int i = 0; i < candidateCount; i++) {
        getline(f, candidates[i].id);
        getline(f, candidates[i].name);
        getline(f, candidates[i].party);
        getline(f, candidates[i].symbol);
        f >> votes[i];
        f.ignore();
    }
}

void LoadVoters() {
    ifstream f("voters.txt");
    if (!f) return;
    f >> voterCount;
    f.ignore();
    for (int i = 0; i < voterCount; i++) {
        getline(f, voters[i].id);
        getline(f, voters[i].name);
        getline(f, voters[i].password);
        f >> voters[i].age >> voters[i].hasVoted >> voters[i].votedFor;
        f.ignore();
    }
}

void DisplayCandidates() {
    cout << "\n--- Candidates ---\n";
    for (int i = 0; i < candidateCount; i++)
        cout << i+1 << ". [" << candidates[i].id << "] " << candidates[i].name 
             << " (" << candidates[i].party << ", " << candidates[i].symbol << ")\n";
}

void DisplayVoters() {
    cout << "\n--- Voters ---\n";
    for (int i = 0; i < voterCount; i++)
        cout << i+1 << ". [" << voters[i].id << "] " << voters[i].name 
             << " (Age: " << voters[i].age << ", Voted: " << (voters[i].hasVoted ? "Yes" : "No") << ")\n";
}

void AddCandidate() {
    if (candidateCount >= MAX_CANDIDATES) { cout << "Limit reached!\n"; Pause(); return; }
    Candidate c;
    cout << "Enter ID: "; cin >> c.id;
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i].id == c.id) { cout << "ID exists!\n"; Pause(); return; }
    }
    cin.ignore();
    cout << "Enter Name: "; getline(cin, c.name);
    cout << "Enter Party: "; getline(cin, c.party);
    cout << "Enter Symbol: "; getline(cin, c.symbol);
    candidates[candidateCount++] = c;
    SaveCandidates();
    cout << "Candidate added!\n";
    Pause();
}

void UpdateCandidate() {
    if (candidateCount == 0) { cout << "No candidates!\n"; Pause(); return; }
    DisplayCandidates();
    string id;
    cout << "\nEnter ID to update: "; cin >> id;
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i].id == id) {
            cin.ignore();
            cout << "New Name: "; getline(cin, candidates[i].name);
            cout << "New Party: "; getline(cin, candidates[i].party);
            cout << "New Symbol: "; getline(cin, candidates[i].symbol);
            SaveCandidates();
            cout << "Updated!\n";
            Pause();
            return;
        }
    }
    cout << "Not found!\n";
    Pause();
}

void DeleteCandidate() {
    if (candidateCount == 0) { cout << "No candidates!\n"; Pause(); return; }
    DisplayCandidates();
    string id;
    cout << "\nEnter ID to delete: "; cin >> id;
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i].id == id) {
            for (int j = i; j < candidateCount-1; j++) {
                candidates[j] = candidates[j+1];
                votes[j] = votes[j+1];
            }
            candidateCount--;
            for (int v = 0; v < voterCount; v++) {
                if (voters[v].votedFor == i) { voters[v].hasVoted = false; voters[v].votedFor = -1; }
                else if (voters[v].votedFor > i) voters[v].votedFor--;
            }
            SaveCandidates();
            SaveVoters();
            cout << "Deleted!\n";
            Pause();
            return;
        }
    }
    cout << "Not found!\n";
    Pause();
}

void AddVoter() {
    if (voterCount >= MAX_VOTERS) { cout << "Limit reached!\n"; Pause(); return; }
    Voter v;
    cout << "Enter CNIC (numbers only): "; cin >> v.id;
    if (!IsNumeric(v.id)) { cout << "Must be numbers!\n"; Pause(); return; }
    for (int i = 0; i < voterCount; i++) {
        if (voters[i].id == v.id) { cout << "ID exists!\n"; Pause(); return; }
    }
    cin.ignore();
    cout << "Enter Name: "; getline(cin, v.name);
    cout << "Set Password: "; cin >> v.password;
    cout << "Enter Age: "; cin >> v.age;
    if (v.age < MIN_AGE) { cout << "Must be 18+!\n"; Pause(); return; }
    voters[voterCount++] = v;
    SaveVoters();
    cout << "Voter added!\n";
    Pause();
}

void UpdateVoter() {
    if (voterCount == 0) { cout << "No voters!\n"; Pause(); return; }
    DisplayVoters();
    string id;
    cout << "\nEnter ID to update: "; cin >> id;
    for (int i = 0; i < voterCount; i++) {
        if (voters[i].id == id) {
            cin.ignore();
            cout << "New Name: "; getline(cin, voters[i].name);
            cout << "New Password: "; cin >> voters[i].password;
            cout << "New Age: "; cin >> voters[i].age;
            SaveVoters();
            cout << "Updated!\n";
            Pause();
            return;
        }
    }
    cout << "Not found!\n";
    Pause();
}

void DeleteVoter() {
    if (voterCount == 0) { cout << "No voters!\n"; Pause(); return; }
    DisplayVoters();
    string id;
    cout << "\nEnter ID to delete: "; cin >> id;
    for (int i = 0; i < voterCount; i++) {
        if (voters[i].id == id) {
            if (voters[i].hasVoted && voters[i].votedFor >= 0) votes[voters[i].votedFor]--;
            for (int j = i; j < voterCount-1; j++) voters[j] = voters[j+1];
            voterCount--;
            SaveVoters();
            SaveCandidates();
            cout << "Deleted!\n";
            Pause();
            return;
        }
    }
    cout << "Not found!\n";
    Pause();
}

void RemoveVote() {
    if (voterCount == 0) { cout << "No voters!\n"; Pause(); return; }
    DisplayVoters();
    string id;
    cout << "\nEnter ID to remove vote: "; cin >> id;
    for (int i = 0; i < voterCount; i++) {
        if (voters[i].id == id) {
            if (!voters[i].hasVoted) { cout << "Hasn't voted!\n"; Pause(); return; }
            votes[voters[i].votedFor]--;
            voters[i].hasVoted = false;
            voters[i].votedFor = -1;
            SaveVoters();
            SaveCandidates();
            cout << "Vote removed!\n";
            Pause();
            return;
        }
    }
    cout << "Not found!\n";
    Pause();
}

void ShowResults() {
    Clear();
    cout << "--- RESULTS ---\n\n";
    if (candidateCount == 0) { cout << "No candidates!\n"; Pause(); return; }
    
    int maxVotes = -1, winnerIndex = -1;
    bool tie = false;
    
    for (int i = 0; i < candidateCount; i++) {
        cout << candidates[i].name << " (" << candidates[i].party << "): " << votes[i] << " votes\n";
        if (votes[i] > maxVotes) {
            maxVotes = votes[i];
            winnerIndex = i;
            tie = false;
        } else if (votes[i] == maxVotes && maxVotes > 0) {
            tie = true;
        }
    }
    
    cout << "\n--- WINNER ---\n";
    if (maxVotes == 0) {
        cout << "No votes cast yet!\n";
    } else if (tie) {
        cout << "It's a TIE! Multiple candidates have " << maxVotes << " votes.\n";
    } else {
        cout << "🏆 " << candidates[winnerIndex].name << " (" << candidates[winnerIndex].party 
             << ") WINS with " << maxVotes << " votes! 🎉\n";
    }
    Pause();
}

void CastVote() {
    Clear();
    string id, name, pass;
    int age;
    cout << "--- CAST VOTE ---\n";
    cout << "CNIC: "; cin >> id;
    if (!IsNumeric(id)) { cout << "Numbers only!\n"; Pause(); return; }
    cin.ignore();
    cout << "Name: "; getline(cin, name);
    cout << "Password: "; cin >> pass;
    cout << "Age: "; cin >> age;
    if (age < MIN_AGE) { cout << "Must be 18+!\n"; Pause(); return; }
    
    int idx = -1;
    for (int i = 0; i < voterCount; i++) {
        if (voters[i].id == id && voters[i].name == name && voters[i].password == pass) {
            if (voters[i].hasVoted) { cout << "Already voted!\n"; Pause(); return; }
            idx = i;
            break;
        }
    }
    if (idx == -1) { cout << "Verification failed!\n"; Pause(); return; }
    
    if (candidateCount == 0) { cout << "No candidates!\n"; Pause(); return; }
    
    DisplayCandidates();
    cout << "\nEnter choice (1-" << candidateCount << "): ";
    int choice;
    cin >> choice;
    if (choice < 1 || choice > candidateCount) { cout << "Invalid!\n"; Pause(); return; }
    
    if (!HumanVerify()) { cout << "Verification failed!\n"; Pause(); return; }
    
    votes[choice-1]++;
    voters[idx].hasVoted = true;
    voters[idx].votedFor = choice-1;
    SaveVoters();
    cout << "\nVote recorded!\n";
    Pause();
}

void StartVoting() {
    if (!votingOngoing) { cout << "Start voting first!\n"; Pause(); return; }
    while (true) {
        Clear();
        CastVote();
        
		}
}

void EndVoting() {
    if (!votingOngoing) { cout << "Voting not started!\n"; Pause(); return; }
    votingOngoing = false;
    SaveCandidates();
    SaveVoters();
    cout << "Voting ended successfully!\n";
    Pause();
}

void ManageCandidates() {
    while (true) {
        Clear();
        cout << "CANDIDATE MANAGEMENT\n1. Add\n2. Update\n3. Delete\n4. Back\nChoice: ";
        int c; cin >> c;
        if (c == 1) AddCandidate();
        else if (c == 2) UpdateCandidate();
        else if (c == 3) DeleteCandidate();
        else if (c == 4) break;
    }
}

void ManageVoters() {
    while (true) {
        Clear();
        cout << "VOTER MANAGEMENT\n1. Add\n2. Update\n3. Delete\n4. Back\nChoice: ";
        int c; cin >> c;
        if (c == 1) AddVoter();
        else if (c == 2) UpdateVoter();
        else if (c == 3) DeleteVoter();
        else if (c == 4) break;
    }
}

void AdminPortal() {
    while (true) {
        Clear();
        cout << "ADMIN PORTAL\n1. Manage Candidates\n2. Manage Voters\n3. Start Voting\n";
        cout << "4. End Voting\n5. Show Results\n6. Remove Vote\n7. Logout\nChoice: ";
        int c; cin >> c;
        if (c == 1) ManageCandidates();
        else if (c == 2) ManageVoters();
        else if (c == 3) { votingOngoing = true; cout << "Voting started!\n"; Pause(); StartVoting(); }
        else if (c == 4) EndVoting();
        else if (c == 5) ShowResults();
        else if (c == 6) RemoveVote();
        else if (c == 7) break;
    }
}

bool AdminLogin() {
    string u, p;
    int age;
    cout << "Username: "; cin >> u;
    cout << "Password: "; cin >> p;
    cout << "Age: "; cin >> age;
    if (age < MIN_AGE) { cout << "Must be 18+!\n"; Pause(); return false; }
    return u == ADMIN_USER && p == ADMIN_PASS;
}

int main() {
    srand(time(0));
    LoadCandidates();
    LoadVoters();
    
    while (true) {
        Clear();
        cout << "ELECTRONIC VOTING SYSTEM\n\n1. Admin Portal\n2. Cast Vote\n3. Exit\nChoice: ";
        int c; cin >> c;
        
        if (c == 1) {
            if (AdminLogin()) AdminPortal();
            else { cout << "Login failed!\n"; Pause(); }
        }
        else if (c == 2) CastVote();
        else if (c == 3) {
            SaveCandidates();
            SaveVoters();
            break;
        }
    }
    return 0;
}