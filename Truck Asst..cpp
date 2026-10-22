#include <iostream>
#include <string>
#include <limits>

using namespace std;

// ==========================================
// CLASS 1: TRUCK
// ==========================================

class Truck
{
public:
    string truckNumber;
    string driverName;
    string contactNumber;

    // Default constructor
    Truck()
    {
        truckNumber = "";
        driverName = "";
        contactNumber = "";
    }

    // Parameterized constructor
    Truck(string number, string name, string contact)
    {
        truckNumber = number;
        driverName = name;
        contactNumber = contact;
    }

    // Operator overloading
    // Used to compare two trucks by registration number
    bool operator<(const Truck &other) const
    {
        return truckNumber < other.truckNumber;
    }
};


// ==========================================
// CLASS 2: ASSISTANCE REQUEST
// ==========================================

class AssistanceRequest
{
public:
    int requestID;
    int sequence;
    string truckNumber;
    string problem;
    string priority;
    string mechanic;
    string status;

    AssistanceRequest()
    {
        requestID = 0;
        sequence = 0;
        truckNumber = "";
        problem = "";
        priority = "";
        mechanic = "";
        status = "Pending";
    }

    AssistanceRequest(int id, int seq,
                      string number,
                      string issue,
                      string level)
    {
        requestID = id;
        sequence = seq;
        truckNumber = number;
        problem = issue;
        priority = level;
        mechanic = "Not Assigned";
        status = "Pending";
    }
};


// ==========================================
// NODE FOR TRUCK LINKED LIST
// ==========================================

class TruckNode
{
public:
    Truck data;
    TruckNode *next;

    TruckNode(Truck t)
    {
        data = t;
        next = NULL;
    }
};


// ==========================================
// NODE FOR REQUEST LINKED LIST
// ==========================================

class RequestNode
{
public:
    AssistanceRequest data;
    RequestNode *next;

    RequestNode(AssistanceRequest r)
    {
        data = r;
        next = NULL;
    }
};


// ==========================================
// MAIN SYSTEM CLASS
// ==========================================

class TruckAssistSystem
{
private:

    TruckNode *truckHead;
    RequestNode *requestHead;

    int nextRequestID;
    int nextSequence;


    // Find a truck using linear search
    TruckNode* findTruck(string number)
    {
        TruckNode *current = truckHead;

        while (current != NULL)
        {
            if (current->data.truckNumber == number)
            {
                return current;
            }

            current = current->next;
        }

        return NULL;
    }


    // Find a request using request ID
    RequestNode* findRequest(int id)
    {
        RequestNode *current = requestHead;

        while (current != NULL)
        {
            if (current->data.requestID == id)
            {
                return current;
            }

            current = current->next;
        }

        return NULL;
    }


    // Find the next pending request.
    // Emergency requests are selected first.
    // Requests with the same priority follow arrival order.
    RequestNode* getNextPendingRequest()
    {
        RequestNode *current = requestHead;
        RequestNode *best = NULL;

        while (current != NULL)
        {
            if (current->data.status == "Pending")
            {
                if (best == NULL)
                {
                    best = current;
                }
                else if (
                    current->data.priority == "Emergency" &&
                    best->data.priority == "Normal"
                )
                {
                    best = current;
                }
                else if (
                    current->data.priority == best->data.priority &&
                    current->data.sequence < best->data.sequence
                )
                {
                    best = current;
                }
            }

            current = current->next;
        }

        return best;
    }


    // Display one assistance request
    void displayRequest(AssistanceRequest r)
    {
        cout << "\nRequest ID: " << r.requestID;
        cout << "\nTruck Number: " << r.truckNumber;
        cout << "\nProblem: " << r.problem;
        cout << "\nPriority: " << r.priority;
        cout << "\nMechanic: " << r.mechanic;
        cout << "\nStatus: " << r.status;
        cout << "\n----------------------------------\n";
    }


public:

    // Constructor
    TruckAssistSystem()
    {
        truckHead = NULL;
        requestHead = NULL;

        nextRequestID = 1001;
        nextSequence = 1;
    }


    // Destructor: release dynamically allocated memory
    ~TruckAssistSystem()
    {
        TruckNode *t;

        while (truckHead != NULL)
        {
            t = truckHead;
            truckHead = truckHead->next;
            delete t;
        }

        RequestNode *r;

        while (requestHead != NULL)
        {
            r = requestHead;
            requestHead = requestHead->next;
            delete r;
        }
    }


    // ======================================
    // 1. REGISTER A NEW TRUCK
    // ======================================

    void registerTruck()
    {
        string number, name, contact;

        cout << "\nEnter Truck Registration Number: ";
        getline(cin, number);

        if (number.empty())
        {
            cout << "Truck number cannot be empty!\n";
            return;
        }

        if (findTruck(number) != NULL)
        {
            cout << "This truck is already registered!\n";
            return;
        }

        cout << "Enter Driver Name: ";
        getline(cin, name);

        cout << "Enter Contact Number: ";
        getline(cin, contact);

        if (name.empty() || contact.empty())
        {
            cout << "Driver name and contact are required!\n";
            return;
        }

        Truck t(number, name, contact);

        TruckNode *newNode = new TruckNode(t);

        // Insert at the end of the linked list
        if (truckHead == NULL)
        {
            truckHead = newNode;
        }
        else
        {
            TruckNode *current = truckHead;

            while (current->next != NULL)
            {
                current = current->next;
            }

            current->next = newNode;
        }

        cout << "\nTruck registered successfully!\n";
    }


    // ======================================
    // 2. DISPLAY ALL TRUCKS
    // ======================================

    void displayTrucks()
    {
        if (truckHead == NULL)
        {
            cout << "\nNo trucks registered.\n";
            return;
        }

        TruckNode *current = truckHead;

        cout << "\n========== REGISTERED TRUCKS ==========\n";

        while (current != NULL)
        {
            cout << "\nTruck Number: "
                 << current->data.truckNumber;

            cout << "\nDriver Name: "
                 << current->data.driverName;

            cout << "\nContact Number: "
                 << current->data.contactNumber;

            cout << "\n----------------------------------\n";

            current = current->next;
        }
    }


    // ======================================
    // 3. SEARCH FOR A TRUCK
    // ======================================

    void searchTruck()
    {
        string number;

        cout << "\nEnter Truck Registration Number: ";
        getline(cin, number);

        TruckNode *result = findTruck(number);

        if (result == NULL)
        {
            cout << "\nTruck not found!\n";
        }
        else
        {
            cout << "\nTruck Found!\n";

            cout << "Truck Number: "
                 << result->data.truckNumber << endl;

            cout << "Driver Name: "
                 << result->data.driverName << endl;

            cout << "Contact Number: "
                 << result->data.contactNumber << endl;
        }
    }


    // ======================================
    // 4. SORT TRUCKS USING BUBBLE SORT
    // ======================================

    void sortTrucks()
    {
        if (truckHead == NULL || truckHead->next == NULL)
        {
            cout << "\nNot enough trucks to sort.\n";
            return;
        }

        bool swapped;
        TruckNode *current;
        TruckNode *last = NULL;

        do
        {
            swapped = false;
            current = truckHead;

            while (current->next != last)
            {
                if (current->next->data < current->data)
                {
                    Truck temp = current->data;

                    current->data = current->next->data;

                    current->next->data = temp;

                    swapped = true;
                }

                current = current->next;
            }

            last = current;

        } while (swapped);

        cout << "\nTrucks sorted by registration number!\n";

        displayTrucks();
    }


    // ======================================
    // 5. CREATE ASSISTANCE REQUEST
    // ======================================

    void requestAssistance()
    {
        string number;
        string issue;
        string priority;

        cout << "\nEnter Truck Registration Number: ";
        getline(cin, number);

        if (findTruck(number) == NULL)
        {
            cout << "\nTruck is not registered!\n";
            cout << "Please register the truck first.\n";
            return;
        }

        int choice;

        cout << "\nSelect Problem:\n";
        cout << "1. Engine Breakdown\n";
        cout << "2. Tyre Puncture\n";
        cout << "3. Fuel Shortage\n";
        cout << "4. Battery Problem\n";
        cout << "5. Towing Required\n";

        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
            case 1:
                issue = "Engine Breakdown";
                break;

            case 2:
                issue = "Tyre Puncture";
                break;

            case 3:
                issue = "Fuel Shortage";
                break;

            case 4:
                issue = "Battery Problem";
                break;

            case 5:
                issue = "Towing Required";
                break;

            default:
                cout << "Invalid problem choice!\n";
                return;
        }

        cout << "\nSelect Priority:\n";
        cout << "1. Normal\n";
        cout << "2. Emergency\n";

        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1)
        {
            priority = "Normal";
        }
        else if (choice == 2)
        {
            priority = "Emergency";
        }
        else
        {
            cout << "Invalid priority choice!\n";
            return;
        }

        AssistanceRequest r(
            nextRequestID,
            nextSequence,
            number,
            issue,
            priority
        );

        RequestNode *newNode = new RequestNode(r);

        // Insert at the end of the request linked list
        if (requestHead == NULL)
        {
            requestHead = newNode;
        }
        else
        {
            RequestNode *current = requestHead;

            while (current->next != NULL)
            {
                current = current->next;
            }

            current->next = newNode;
        }

        cout << "\nAssistance request created successfully!\n";
        cout << "Request ID: " << nextRequestID << endl;
        cout << "Priority: " << priority << endl;
        cout << "Status: Pending\n";

        nextRequestID++;
        nextSequence++;
    }


    // ======================================
    // 6. DISPLAY ALL ASSISTANCE REQUESTS
    // ======================================

    void displayRequests()
    {
        if (requestHead == NULL)
        {
            cout << "\nNo assistance requests available.\n";
            return;
        }

        RequestNode *current = requestHead;

        cout << "\n========== ALL ASSISTANCE REQUESTS ==========\n";

        while (current != NULL)
        {
            displayRequest(current->data);

            current = current->next;
        }
    }


    // ======================================
    // 7. ASSIGN MECHANIC
    // ======================================

    void assignMechanic()
    {
        RequestNode *r = getNextPendingRequest();

        if (r == NULL)
        {
            cout << "\nNo pending requests available.\n";
            return;
        }

        cout << "\nNext request selected by priority:\n";

        displayRequest(r->data);

        string mechanicName;

        cout << "Enter Mechanic Name: ";
        getline(cin, mechanicName);

        if (mechanicName.empty())
        {
            cout << "Mechanic name cannot be empty!\n";
            return;
        }

        r->data.mechanic = mechanicName;
        r->data.status = "Assigned";

        cout << "\nMechanic assigned successfully!\n";
        cout << "Request ID: " << r->data.requestID << endl;
        cout << "Assigned Mechanic: "
             << r->data.mechanic << endl;
    }


    // ======================================
    // 8. COMPLETE ASSISTANCE REQUEST
    // ======================================

    void completeRequest()
    {
        int id;

        cout << "\nEnter Request ID: ";
        cin >> id;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        RequestNode *r = findRequest(id);

        if (r == NULL)
        {
            cout << "\nRequest not found!\n";
            return;
        }

        if (r->data.status != "Assigned")
        {
            cout << "\nOnly assigned requests can be completed.\n";
            return;
        }

        r->data.status = "Completed";

        cout << "\nAssistance completed successfully!\n";
        cout << "Request ID: " << id << endl;
    }


    // ======================================
    // 9. CANCEL A PENDING REQUEST
    // ======================================

    void cancelRequest()
    {
        int id;

        cout << "\nEnter Request ID to cancel: ";
        cin >> id;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        RequestNode *r = findRequest(id);

        if (r == NULL)
        {
            cout << "\nRequest not found!\n";
            return;
        }

        if (r->data.status != "Pending")
        {
            cout << "\nOnly pending requests can be cancelled.\n";
            return;
        }

        r->data.status = "Cancelled";

        cout << "\nRequest cancelled successfully!\n";
    }


    // ======================================
    // 10. DISPLAY REQUESTS BY PRIORITY
    // ======================================

    void displayPriorityRequests()
    {
        RequestNode *current = requestHead;
        int count = 0;

        cout << "\n========== EMERGENCY REQUESTS ==========\n";

        while (current != NULL)
        {
            if (current->data.status == "Pending" &&
                current->data.priority == "Emergency")
            {
                displayRequest(current->data);
                count++;
            }

            current = current->next;
        }

        if (count == 0)
        {
            cout << "No pending emergency requests.\n";
        }

        current = requestHead;
        count = 0;

        cout << "\n========== NORMAL REQUESTS ==========\n";

        while (current != NULL)
        {
            if (current->data.status == "Pending" &&
                current->data.priority == "Normal")
            {
                displayRequest(current->data);
                count++;
            }

            current = current->next;
        }

        if (count == 0)
        {
            cout << "No pending normal requests.\n";
        }
    }


    // ======================================
    // 11. DISPLAY COMPLETED REQUEST HISTORY
    // ======================================

    void displayHistory()
    {
        RequestNode *current = requestHead;
        int count = 0;

        cout << "\n========== COMPLETED REQUEST HISTORY ==========\n";

        while (current != NULL)
        {
            if (current->data.status == "Completed")
            {
                displayRequest(current->data);
                count++;
            }

            current = current->next;
        }

        if (count == 0)
        {
            cout << "No completed requests available.\n";
        }
    }
};


// ==========================================
// MAIN FUNCTION
// ==========================================

int main()
{
    TruckAssistSystem system;

    int choice;

    do
    {
        cout << "\n\n";
        cout << "========================================\n";
        cout << "          TRUCK ASSIST SYSTEM           \n";
        cout << "========================================\n";

        cout << "1. Register New Truck\n";
        cout << "2. Display All Trucks\n";
        cout << "3. Search Truck\n";
        cout << "4. Sort Trucks\n";
        cout << "5. Request Assistance\n";
        cout << "6. Display All Requests\n";
        cout << "7. Assign Mechanic\n";
        cout << "8. Complete Assistance\n";
        cout << "9. Cancel Request\n";
        cout << "10. Display Requests by Priority\n";
        cout << "11. View Completed History\n";
        cout << "0. Exit\n";

        cout << "========================================\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nInvalid input! Enter a number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
            case 1:
                system.registerTruck();
                break;

            case 2:
                system.displayTrucks();
                break;

            case 3:
                system.searchTruck();
                break;

            case 4:
                system.sortTrucks();
                break;

            case 5:
                system.requestAssistance();
                break;

            case 6:
                system.displayRequests();
                break;

            case 7:
                system.assignMechanic();
                break;

            case 8:
                system.completeRequest();
                break;

            case 9:
                system.cancelRequest();
                break;

            case 10:
                system.displayPriorityRequests();
                break;

            case 11:
                system.displayHistory();
                break;

            case 0:
                cout << "\nThank you for using Truck Assist System!\n";
                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
