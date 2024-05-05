#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

const char FILE_DELIMITER = ',';
const char COMMAND_DELIMITER = ' ';
const char SIGN_1 = ':';
const string SIGN_2 = "->";

class Worker;
class Stage;

class Car
{
    private:
        int arrival_time, ID;
        Worker* worker;
        Stage* current_stage;
        vector<Stage*> stages;
        bool on_stage = false;
    public:
        Car(int arrival_time, int ID, Worker* worker,
        Stage* current_stage, vector<Stage*> stages, bool on_stage)
        {
            this->arrival_time = arrival_time;
            this->ID = ID;
            this->worker = worker;
            this->current_stage = current_stage;
            this->stages = stages;
            this->on_stage = on_stage;
        }
        int get_ID() {return ID;}
        Stage* get_current_stage() {return current_stage;}
        vector<Stage*>& get_stages() {return stages;}
        Worker* get_worker() {return worker;}
        void set_is_on_stage(string value) {if (value == "Yes") on_stage = true; else on_stage = false;}
        void set_worker(Worker* worker) {this->worker = worker;}
        void set_arrival_time(int value) {arrival_time = value;}
        bool is_on_stage() {return on_stage;}
        // int get_ID() {return ID;}
        // int get_time() {return time;}
        // vector<int>& get_stages() {return stages;}
};

class Worker
{
    private:
        int ID;
        int stage_ID;
        int time_to_finish;
        int remaining_time;
        bool busy = false;
        Car* car;
    public:
        void set_ID(int value) {ID = value;}
        void set_stage_ID(int value) {stage_ID = value;}
        void set_time_to_finish(int value) {time_to_finish = value;}
        void set_remaining_time() {remaining_time = time_to_finish;}
        int get_stage_ID() {return stage_ID;}
        int get_ID() {return ID;}
        bool is_busy() {return busy;}
        void set_busy(string value) {if (value == "Yes") busy = true; else busy = false;}
        void set_car(Car* value) {car = value;}
        void pass_time(int value) {remaining_time -= value;}
        int get_time_to_finish() {return time_to_finish;}
        int get_remaining_time() {return remaining_time;}
        Car* get_car() {return car;}
        // int get_ID() {return ID;}
        // int get_time_to_finish() {return time_to_finish;}
        // int get_ID() {return ID;}
        // int get_time() {return time_to_finish;}
};

class Stage
{
    private:
        int ID, price, income;
        vector<Worker*> workers_list;
        vector<Car*> cars_list;
    public:
        void set_ID(int value) {ID = value;}
        void set_price(int value) {price = value;}
        int get_ID() {return ID;}
        bool is_full();
        void set_car(Car* value) {cars_list.push_back(value);}
        vector<Worker*>& get_workers_list() {return workers_list;}
        vector<Car*>& get_cars_list() {return cars_list;}
        // int get_price() {return price;}
};

class Carwash
{
    private:
        int time, num_of_cars;
        vector<Car*> cars_list;
        vector<Stage> stages_list;
    public:
        Carwash(int time, int num_of_cars, vector<Stage> stages_list)
        {
            this->time = time;
            this->num_of_cars = num_of_cars;
            this->stages_list = stages_list;
        }
        void set_car(Car* value) {cars_list.push_back(value);}
        void pass_time(int increase);
        void set_car_next_position(Car* car);
        void car_arrival(vector<int> stages);
        void get_stage_status();
        void get_worker_status();
        void get_car_status();
        //int get_time() {return time;}
        //vector<Stage>& get_stage_list() {return stages_list;}
        //vector<Car>& get_car_list() {return cars_list;}
        // int num_of_busy_workers()
        // {
        //     int num = 0;
        //     for (int i = 0; i < stages_list.size(); i++)
        //     {
        //         for (int j = 0; j < stages_list[i].get_workers_list().size(); j++)
        //         {
        //             if (stages_list[i].get_workers_list()[j]->is_busy())
        //                 num++;
        //         }
        //     }
        //     return num;
        // }
};

vector<Stage> read_stages_file(char* argv[]);
vector<Worker> read_workers_file(char* argv[]);
void set_workers_on_stages(vector<Stage>& stages_list ,vector<Worker>& workers_list);
void receive_commands(Carwash& carwash);
void do_commands(Carwash& carwash, string method, const vector<int>& arguments_list);

int main(int argc, char* argv[])
{
    vector<Car> cars_list;
    vector<Stage> stages_list = read_stages_file(argv);
    vector<Worker> workers_list = read_workers_file(argv);
    set_workers_on_stages(stages_list, workers_list);
    Carwash carwash(0, 0, stages_list);

    // for (int i = 0; i < carwash.get_stage_list().size(); i++)
    // {
    //     for (int j = 0; j < carwash.get_stage_list()[i].get_workers_list().size(); j++)
    //     {
    //         cout << carwash.get_stage_list()[i].get_workers_list()[j].get_ID() << ' '
    //              << carwash.get_stage_list()[i].get_workers_list()[j].get_stage_ID() << ' '
    //              << carwash.get_stage_list()[i].get_workers_list()[j].get_time() << endl;
    //     }
    // }

    receive_commands(carwash);

    // for (int i = 0; i < carwash.get_car_list().size(); i++)
    // {
    //     cout << carwash.get_car_list()[i].get_ID() << ' '
    //          << carwash.get_car_list()[i].get_time() << ' '
    //          << carwash.get_car_list()[i].get_stages().size() << endl;
    // }

    return 0;
}

vector<Stage> read_stages_file(char* argv[])
{
    ifstream stages(argv[1]);
    string stage_parameters, ID_or_price_string, line;
    int num_of_lines = 0;
    int line_num = 0;
    int ID_or_price;
    bool is_first_line = true;
    bool is_ID = true;
    while (getline(stages, line)) num_of_lines++;
    num_of_lines--;
    vector<Stage> stages_list(num_of_lines);
    stages.clear();
    stages.seekg(0);
    while (getline(stages, stage_parameters))
    {
        if (is_first_line)
        {
            is_first_line = false;
            continue;
        }
        stringstream stage_parameters_stream(stage_parameters);
        while (getline(stage_parameters_stream, ID_or_price_string, FILE_DELIMITER))
        {
            ID_or_price = stoi(ID_or_price_string);
            if (is_ID) stages_list[line_num].set_ID(ID_or_price);
            else stages_list[line_num].set_price(ID_or_price);
            is_ID = false;
        }
        is_ID = true;
        line_num++;
    }
    stages.close();
    return stages_list;
}

vector<Worker> read_workers_file(char* argv[])
{
    ifstream workers(argv[2]);
    string worker_parameters, single_parameter_string, line;
    int num_of_lines = 0;
    int line_num = 0;
    int single_parameter;
    int single_parameter_counter = 0;
    bool is_first_line = true;
    while (getline(workers, line)) num_of_lines++;
    num_of_lines--;
    vector<Worker> workers_list(num_of_lines);
    workers.clear();
    workers.seekg(0);
    while (getline(workers, worker_parameters))
    {
        if (is_first_line)
        {
            is_first_line = false;
            continue;
        }
        stringstream worker_parameters_stream(worker_parameters);
        while (getline(worker_parameters_stream, single_parameter_string, FILE_DELIMITER))
        {
            single_parameter = stoi(single_parameter_string);
            if (single_parameter_counter == 0) workers_list[line_num].set_ID(single_parameter);
            else if (single_parameter_counter == 1) workers_list[line_num].set_stage_ID(single_parameter);
            else
            { 
                workers_list[line_num].set_time_to_finish(single_parameter);
                workers_list[line_num].set_remaining_time();
            }
            single_parameter_counter++;
        }
        single_parameter_counter = 0;
        line_num++;
    }
    workers.close();
    return workers_list;
}

void set_workers_on_stages(vector<Stage>& stages_list ,vector<Worker>& workers_list)
{
    for (int i = 0; i < workers_list.size(); i++)
    {
        for (int j = 0; j < stages_list.size(); j++)
        {
            if (workers_list[i].get_stage_ID() == stages_list[j].get_ID())
            {
                stages_list[j].get_workers_list().push_back(&workers_list[i]);
            }
        }
    }
}

void receive_commands(Carwash& carwash)
{
    int argument;
    string command, command_parameters, method;
    vector<int> arguments_list;
    bool is_first_parameter = true;
    while (getline(cin, command))
    {
        if (command == "0") break;
        arguments_list.clear();
        stringstream command_stream(command);
        while (getline(command_stream, command_parameters, COMMAND_DELIMITER))
        {
            if (is_first_parameter) method = command_parameters;
            else
            {
                argument = stoi(command_parameters);
                arguments_list.push_back(argument);
            }
            is_first_parameter = false;
        }
        is_first_parameter = true;
        do_commands(carwash, method, arguments_list);
    }
}

void do_commands(Carwash& carwash, string method, const vector<int>& arguments_list)
{
    if (method == "pass_time") carwash.pass_time(arguments_list[0]);
    else if (method == "car_arrival") carwash.car_arrival(arguments_list);
}

bool Stage::is_full()
{
    int num_of_busy_workers = 0;
    for (int i = 0; i < get_workers_list().size(); i++)
    {
        if (get_workers_list()[i]->is_busy())
            num_of_busy_workers++;
    }
    if (num_of_busy_workers == get_workers_list().size())
        return true;
}

void Carwash::pass_time(int increase)
{
    Car* car;
    for (int t = 0; t < increase; t++)
    {
        time++;
        for (int i = 0; i < stages_list.size(); i++)
        {
            for (int j = 0; j < stages_list[i].get_cars_list().size(); j++)
            {
                car = stages_list[i].get_cars_list()[j];
                if (car->is_on_stage())
                {
                    car->get_worker()->pass_time(1);
                    if (car->get_worker()->get_remaining_time() == 0)
                    {
                        car->get_worker()->set_busy("No");
                        car->get_worker()->set_remaining_time();
                        car->get_worker() == nullptr;
                        car->get_worker()->get_car() == nullptr;
                        set_car_next_position(car);
                    }
                }
                else
                {
                    if (!car->get_current_stage()->is_full())
                        set_car_next_position(car);
                }
            }
        }
    }
}

void Carwash::car_arrival(vector<int> stages)
{
    int ID = num_of_cars + 1;
    num_of_cars++;
    vector<Stage*> car_stages;
    Stage* car_current_stage;
    Worker* car_worker = nullptr;
    bool on_stage;
    for (int i = 0; i < stages.size(); i++)
    {
        for (int j = 0; j < stages_list.size(); j++)
        {
            if (stages[i] == stages_list[j].get_ID())
            {
                car_stages.push_back(&stages_list[j]);
            }
        }
    }
    car_current_stage = car_stages[0];
    if (car_current_stage->is_full()) on_stage = false;
    else
    {
        on_stage = true;
        for (int i = 0; i < car_current_stage->get_workers_list().size(); i++)
        {
            if (!car_current_stage->get_workers_list()[i]->is_busy())
            {
                car_worker = car_current_stage->get_workers_list()[i];
                break;
            }
        }
        for (int i = 0; i < car_current_stage->get_workers_list().size(); i++)
        {
            if (!car_current_stage->get_workers_list()[i]->is_busy() &&
            car_current_stage->get_workers_list()[i]->get_time_to_finish() < car_worker->get_time_to_finish())
            car_worker = car_current_stage->get_workers_list()[i];
            else if (!car_current_stage->get_workers_list()[i]->is_busy() &&
            car_current_stage->get_workers_list()[i]->get_time_to_finish() == car_worker->get_time_to_finish())
            {
                if (car_current_stage->get_workers_list()[i]->get_ID() < car_worker->get_ID())
                    car_worker = car_current_stage->get_workers_list()[i];
            }
        }
        car_worker->set_busy("Yes");
    }
    Car car(time, ID, car_worker, car_current_stage, car_stages, on_stage);
    car_current_stage->set_car(&car);
    set_car(&car);
    car_worker->set_car(&car);
    cout << time << " Car " << ID << SIGN_1 << " Arrived " << SIGN_2;
    if (on_stage) cout << " Stage " << car_current_stage->get_ID() << endl;
    else cout << " Queue " << car_current_stage->get_ID() << endl;
}

void Carwash::get_stage_status(){}

void Carwash::get_worker_status(){}

void Carwash::get_car_status(){}

void Carwash::set_car_next_position(Car* car)
{
    Stage* next_stage;
    Worker* new_worker;
    string previous_position, current_position;
    if (car->is_on_stage()) previous_position = "Stage";
    else previous_position = "Queue";

    for (int i = 0; i < car->get_stages().size(); i++)
    {
        if (car->get_current_stage() == car->get_stages()[i])
        {
            if (i < car->get_stages().size() && car->is_on_stage())
                next_stage = car->get_stages()[i + 1];
            else if (i <= car->get_stages().size() && !car->is_on_stage())
                next_stage = car->get_stages()[i];
            else next_stage = nullptr;
        }
    }
    if (next_stage->is_full() && next_stage != nullptr)
    {
        car->set_is_on_stage("No");
        current_position = "Queue";
    }
    else if (next_stage != nullptr)
    {
        car->set_is_on_stage("Yes");
        current_position = "Stage";
        for (int i = 0; i < next_stage->get_workers_list().size(); i++)
        {
            if (!next_stage->get_workers_list()[i]->is_busy())
            {
                new_worker = next_stage->get_workers_list()[i];
                car->set_worker(new_worker);
                break;
            }
        }
        new_worker->set_busy("Yes");
        new_worker->set_car(car);
    }
    else current_position = "Done";
    car->set_arrival_time(time);
    cout << time << " Car " << car->get_ID() << SIGN_1 << ' '
         << previous_position << ' ' << car->get_current_stage()->get_ID()
         << ' ' << SIGN_2 << ' ' << current_position;
    if (next_stage != nullptr) cout << ' ' << next_stage->get_ID() << endl;
    else cout << endl;
}