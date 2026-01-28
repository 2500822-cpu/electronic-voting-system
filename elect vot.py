import random
import os

MAX_CANDIDATES = 5
MAX_VOTERS = 75
ADMIN_USERNAME = "admin"
ADMIN_PASSWORD = "password"
MINIMUM_AGE = 18

candidates = []
voters = []
votes = []
voting_ongoing = False

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

def pause(msg="Press Enter to continue..."):
    input(msg)

def save_data():
    try:
        with open("candidates.txt", "w") as f:
            f.write(f"{len(candidates)}\n")
            for i, c in enumerate(candidates):
                f.write(f"{c['id']}\n{c['name']}\n{c['party']}\n{c['symbol']}\n{votes[i]}\n")
    except: pass
    
    try:
        with open("voters.txt", "w") as f:
            f.write(f"{len(voters)}\n")
            for v in voters:
                f.write(f"{v['id']}\n{v['name']}\n{v['pass']}\n{v['age']}\n{1 if v['voted'] else 0}\n{v['votedFor']}\n")
    except: pass

def load_data():
    global candidates, voters, votes
    candidates, voters, votes = [], [], []
    
    try:
        with open("candidates.txt", "r") as f:
            count = min(max(int(f.readline().strip()), 0), MAX_CANDIDATES)
            for _ in range(count):
                candidates.append({'id': f.readline().strip(), 'name': f.readline().strip(), 
                                 'party': f.readline().strip(), 'symbol': f.readline().strip()})
                votes.append(max(int(f.readline().strip()), 0))
    except: pass
    
    try:
        with open("voters.txt", "r") as f:
            count = min(max(int(f.readline().strip()), 0), MAX_VOTERS)
            for _ in range(count):
                v = {'id': f.readline().strip(), 'name': f.readline().strip(), 'pass': f.readline().strip(),
                     'age': int(f.readline().strip()), 'voted': bool(int(f.readline().strip())), 
                     'votedFor': int(f.readline().strip())}
                if v['votedFor'] < 0 or v['votedFor'] >= len(candidates):
                    v['votedFor'], v['voted'] = -1, False
                voters.append(v)
    except: pass

def human_verify():
    pin = random.randint(1000, 9999)
    print(f"Human Verification - Enter PIN: {pin}")
    try:
        return int(input("PIN: ")) == pin
    except:
        return False

def login_admin():
    username = input("Admin Username: ")
    password = input("Admin Password: ")
    try:
        age = int(input("Your Age: "))
        if age < MINIMUM_AGE:
            print(f"Must be {MINIMUM_AGE}+ to be admin.")
            pause()
            return False
    except:
        print("Invalid age.")
        pause()
        return False
    return username == ADMIN_USERNAME and password == ADMIN_PASSWORD

def show_candidates():
    print("\n--- Candidates ---")
    for i, c in enumerate(candidates):
        print(f"{i+1}. [{c['id']}] {c['name']} ({c['party']}, {c['symbol']})")

def show_voters():
    print("\n--- Voters ---")
    for i, v in enumerate(voters):
        print(f"{i+1}. [{v['id']}] {v['name']} (Age: {v['age']}, Voted: {'Yes' if v['voted'] else 'No'})")

def add_candidate():
    if len(candidates) >= MAX_CANDIDATES:
        print("Candidate limit reached.")
        pause()
        return
    
    cid = input("Candidate ID: ")
    if any(c['id'] == cid for c in candidates):
        print("ID already exists!")
        pause()
        return
    
    candidates.append({'id': cid, 'name': input("Name: "), 
                      'party': input("Party: "), 'symbol': input("Symbol: ")})
    votes.append(0)
    save_data()
    print("Candidate added.")
    pause()

def update_candidate():
    if not candidates:
        print("No candidates.")
        pause()
        return
    show_candidates()
    cid = input("\nCandidate ID to update: ")
    for c in candidates:
        if c['id'] == cid:
            c['name'], c['party'], c['symbol'] = input("New Name: "), input("New Party: "), input("New Symbol: ")
            save_data()
            print("Updated.")
            pause()
            return
    print("Not found.")
    pause()

def delete_candidate():
    if not candidates:
        print("No candidates.")
        pause()
        return
    show_candidates()
    cid = input("\nCandidate ID to delete: ")
    for i, c in enumerate(candidates):
        if c['id'] == cid:
            for v in voters:
                if v['votedFor'] == i:
                    v['voted'], v['votedFor'] = False, -1
                elif v['votedFor'] > i:
                    v['votedFor'] -= 1
            candidates.pop(i)
            votes.pop(i)
            save_data()
            print("Deleted.")
            pause()
            return
    print("Not found.")
    pause()

def manage_candidates():
    while True:
        clear_screen()
        print("CANDIDATE MANAGEMENT\n1. Add Candidates\n2. Update Candidate\n3. Delete candidates\n4. Back")
        try:
            choice = int(input("Choice: "))
            if choice == 1: add_candidate()
            elif choice == 2: update_candidate()
            elif choice == 3: delete_candidate()
            elif choice == 4: break
            else: print("Invalid!"); pause()
        except: print("Invalid!"); pause()

def add_voter():
    if len(voters) >= MAX_VOTERS:
        print("Voter limit reached.")
        pause()
        return
    
    vid = input("Voter ID (CNIC): ")
    if not vid.isdigit():
        print("CNIC must be digits!")
        pause()
        return
    if any(v['id'] == vid for v in voters):
        print("ID exists!")
        pause()
        return
    
    name, password = input("Name: "), input("Password: ")
    try:
        age = int(input("Age: "))
        if age < MINIMUM_AGE:
            print(f"Must be {MINIMUM_AGE}+")
            pause()
            return
    except:
        print("Invalid age.")
        pause()
        return
    
    voters.append({'id': vid, 'name': name, 'pass': password, 'age': age, 'voted': False, 'votedFor': -1})
    save_data()
    print("Voter added.")
    pause()

def update_voter():
    if not voters:
        print("No voters.")
        pause()
        return
    show_voters()
    vid = input("\nVoter ID to update: ")
    for v in voters:
        if v['id'] == vid:
            v['name'], v['pass'] = input("New Name: "), input("New Password: ")
            try:
                v['age'] = int(input("New Age: "))
                save_data()
                print("Updated.")
            except:
                print("Invalid age.")
            pause()
            return
    print("Not found.")
    pause()

def delete_voter():
    if not voters:
        print("No voters.")
        pause()
        return
    show_voters()
    vid = input("\nVoter ID to delete: ")
    for i, v in enumerate(voters):
        if v['id'] == vid:
            if v['voted'] and 0 <= v['votedFor'] < len(candidates):
                votes[v['votedFor']] = max(0, votes[v['votedFor']] - 1)
            voters.pop(i)
            save_data()
            print("Deleted.")
            pause()
            return
    print("Not found.")
    pause()

def manage_voters():
    while True:
        clear_screen()
        print("VOTER MANAGEMENT\n1. Add Voters\n2. Update Voters\n3. Delete Voters\n4. Back")
        try:
            choice = int(input("Choice: "))
            if choice == 1: add_voter()
            elif choice == 2: update_voter()
            elif choice == 3: delete_voter()
            elif choice == 4: break
            else: print("Invalid!"); pause()
        except: print("Invalid!"); pause()

def show_results():
    clear_screen()
    print("--- VOTING RESULTS ---\n")
    if not candidates:
        print("No candidates.")
        pause()
        return
    for i, c in enumerate(candidates):
        print(f"{c['name']} ({c['party']}, {c['symbol']}): {votes[i]} votes")
    pause()

def cast_vote(voter_idx):
    clear_screen()
    print("--- CAST YOUR VOTE ---\n")
    if not candidates:
        print("No candidates available.")
        pause()
        return
    
    show_candidates()
    try:
        choice = int(input("\nEnter candidate number: "))
        if choice < 1 or choice > len(candidates):
            print("Invalid choice.")
            pause()
            return
    except:
        print("Invalid input.")
        pause()
        return
    
    if not human_verify():
        print("Verification failed.")
        pause()
        return
    
    votes[choice - 1] += 1
    voters[voter_idx]['voted'], voters[voter_idx]['votedFor'] = True, choice - 1
    save_data()
    print("Vote recorded!")
    if input("\nView results? (Y/N): ").upper() == 'Y':
        show_results()
    else:
        pause()

def voter_interface():
    clear_screen()
    print("--- VOTER INTERFACE ---")
    vid = input("CNIC: ")
    if not vid.isdigit():
        print("CNIC must be digits!")
        pause()
        return
    
    name, password = input("Name: "), input("Password: ")
    try:
        age = int(input("Age: "))
    except:
        print("Invalid age.")
        pause()
        return
    
    if age < MINIMUM_AGE:
        print(f"Must be {MINIMUM_AGE}+")
        pause()
        return
    
    for i, v in enumerate(voters):
        if v['id'] == vid and v['name'] == name and v['pass'] == password:
            if v['voted']:
                print("Already voted!")
                pause()
                return
            v['age'] = age
            cast_vote(i)
            return
    
    print("Verification failed!")
    pause()

def start_voting():
    if not voting_ongoing:
        print("Start voting first.")
        pause()
        return
    
    while True:
        clear_screen()
        print("INTERACTIVE VOTING (Enter 'EXIT' or 0 to stop)\n")
        vid = input("Voter ID: ")
        if vid.upper() == "EXIT" or vid == "0":
            break
        
        if not vid.isdigit():
            print("Digits only!")
            pause()
            continue
        
        password = input("Password: ")
        voter_idx = -1
        for i, v in enumerate(voters):
            if v['id'] == vid and v['pass'] == password and not v['voted']:
                voter_idx = i
                break
        
        if voter_idx == -1:
            print("Invalid or already voted.")
            pause()
            continue
        
        if not human_verify():
            print("Verification failed.")
            pause()
            continue
        
        show_candidates()
        try:
            choice = int(input("Candidate number: "))
            if choice < 1 or choice > len(candidates):
                print("Invalid!")
                pause()
                continue
        except:
            print("Invalid!")
            pause()
            continue
        
        votes[choice - 1] += 1
        voters[voter_idx]['voted'], voters[voter_idx]['votedFor'] = True, choice - 1
        save_data()
        print("Vote recorded!")
        if input("View results? (Y/N): ").upper() == 'Y':
            show_results()
        else:
            pause()

def remove_vote():
    if not voters:
        print("No voters.")
        pause()
        return
    show_voters()
    vid = input("\nVoter ID: ")
    for v in voters:
        if v['id'] == vid:
            if not v['voted'] or v['votedFor'] < 0 or v['votedFor'] >= len(candidates):
                print("No valid vote to remove.")
            else:
                votes[v['votedFor']] = max(0, votes[v['votedFor']] - 1)
                v['voted'], v['votedFor'] = False, -1
                save_data()
                print("Vote removed.")
            pause()
            return
    print("Not found.")
    pause()

def admin_portal():
    global voting_ongoing
    while True:
        clear_screen()
        print("ADMIN PORTAL\n1. Manage Candidates\n2. Manage Voters\n3. Start Voting\n4. End Voting\n5. Show Results\n6. Remove Vote\n7. Logout")
        try:
            choice = int(input("Choice: "))
            if choice == 1: manage_candidates()
            elif choice == 2: manage_voters()
            elif choice == 3:
                voting_ongoing = True
                print("Voting started.")
                pause()
                start_voting()
            elif choice == 4:
                if not voting_ongoing:
                    print("Not ongoing.")
                else:
                    voting_ongoing = False
                    print("Voting ended.")
                    save_data()
                pause()
            elif choice == 5: show_results()
            elif choice == 6: remove_vote()
            elif choice == 7: break
            else: print("Invalid!"); pause()
        except: print("Invalid!"); pause()

def main():
    random.seed()
    load_data()
    
    while True:
        clear_screen()
        print("ELECTRONIC VOTING SYSTEM\n\n1. Admin Portal\n2. Voter Interface\n3. Exit")
        try:
            choice = int(input("Choice: "))
            if choice == 1:
                if login_admin():
                    admin_portal()
                else:
                    print("Login failed!")
                    pause()
            elif choice == 2:
                voter_interface()
            elif choice == 3:
                save_data()
                break
            else:
                print("Invalid!")
                pause()
        except:
            print("Invalid!")
            pause()

if __name__ == "__main__":
    main()