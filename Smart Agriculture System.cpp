#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class Crop {
private:
    int cropID;
    string name;
    string soilType;
    int moistureLevel; // 0-100%
    int estimatedYield; // kg per acre

public:
    Crop(int id, string cname, string soil, int moisture, int yield) {
        cropID = id;
        name = cname;
        soilType = soil;
        moistureLevel = moisture;
        estimatedYield = yield;
    }

    int getCropID() const { return cropID; }
    string getName() const { return name; }
    string getSoilType() const { return soilType; }
    int getMoistureLevel() const { return moistureLevel; }
    int getEstimatedYield() const { return estimatedYield; }

    void updateMoisture(int level) {
        moistureLevel = level;
    }

    void displayCrop() const {
        cout << "ID: " << cropID << " | Name: " << name << " | Soil: " << soilType
             << " | Moisture: " << moistureLevel << "% | Yield: " << estimatedYield << " kg/acre" << endl;
    }

    void saveToFile(ofstream &outfile) const {
        outfile << cropID << "," << name << "," << soilType << "," << moistureLevel << "," << estimatedYield << endl;
    }

    static Crop loadFromFile(ifstream &infile) {
        int id, moisture, yield;
        string name, soil;
        infile >> id;
        infile.ignore();
        getline(infile, name, ',');
        getline(infile, soil, ',');
        infile >> moisture >> yield;
        return Crop(id, name, soil, moisture, yield);
    }
};

bool isCropIDUnique(vector<Crop> &crops, int id) {
    for (const auto &c : crops) {
        if (c.getCropID() == id) {
            return false;
        }
    }
    return true;
}

Crop* findCrop(vector<Crop> &crops, int id) {
    for (auto &c : crops) {
        if (c.getCropID() == id) {
            return &c;
        }
    }
    return nullptr;
}

void saveCropsToFile(vector<Crop> &crops) {
    ofstream outfile("crops.txt", ios::trunc);
    for (const auto &c : crops) {
        c.saveToFile(outfile);
    }
    outfile.close();
}

void loadCropsFromFile(vector<Crop> &crops) {
    ifstream infile("crops.txt");
    if (!infile) return;

    while (!infile.eof()) {
        int id, moisture, yield;
        string name, soil;

        infile >> id;
        infile.ignore();
        if (getline(infile, name, ',') && getline(infile, soil, ',') && infile >> moisture >> yield) {
            crops.push_back(Crop(id, name, soil, moisture, yield));
        }
    }
    infile.close();
}

void deleteCrop(vector<Crop> &crops, int id) {
    for (auto it = crops.begin(); it != crops.end(); ++it) {
        if (it->getCropID() == id) {
            crops.erase(it);
            cout << "Crop deleted successfully!\n";
            saveCropsToFile(crops);
            return;
        }
    }
    cout << "Crop not found!\n";
}

int main() {
    vector<Crop> crops;
    loadCropsFromFile(crops);

    int choice;
    while (true) {
        cout << "\n===== AI-Powered Smart Agriculture System =====\n";
        cout << "1. Register a Crop\n";
        cout << "2. View All Crops\n";
        cout << "3. Update Soil Moisture Level\n";
        cout << "4. Delete a Crop Record\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, moisture, yield;
            string name, soil;
            cout << "Enter Crop ID: ";
            cin >> id;
            cin.ignore();

            if (!isCropIDUnique(crops, id)) {
                cout << "Error! Crop ID already exists.\n";
                continue;
            }

            cout << "Enter Crop Name: ";
            getline(cin, name);
            cout << "Enter Soil Type: ";
            getline(cin, soil);
            cout << "Enter Moisture Level (0-100%): ";
            cin >> moisture;
            cout << "Enter Estimated Yield (kg/acre): ";
            cin >> yield;

            crops.push_back(Crop(id, name, soil, moisture, yield));
            saveCropsToFile(crops);
            cout << "Crop Registered Successfully!\n";

        } else if (choice == 2) {
            cout << "\n===== Registered Crops =====\n";
            for (const auto &c : crops) {
                c.displayCrop();
            }

        } else if (choice == 3) {
            int id, moisture;
            cout << "Enter Crop ID to update moisture level: ";
            cin >> id;
            Crop* crop = findCrop(crops, id);

            if (crop) {
                cout << "Enter New Moisture Level (0-100%): ";
                cin >> moisture;
                crop->updateMoisture(moisture);
                saveCropsToFile(crops);
                cout << "Moisture level updated successfully!\n";
            } else {
                cout << "Crop not found!\n";
            }

        } else if (choice == 4) {
            int id;
            cout << "Enter Crop ID to delete: ";
            cin >> id;
            deleteCrop(crops, id);

        } else if (choice == 5) {
            cout << "Exiting Smart Agriculture System. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
