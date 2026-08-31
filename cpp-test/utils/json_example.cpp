#include <network.hpp>
#include <ucaspian.hpp>
#include <utils/json_helpers.hpp>

int main()
{
    const std::string backend = "ucaspian";
    const std::string network_filename = "networks/passnet.json";

    const int w = 5;
    const int h = 5;
    const int runs = 10;

    std::unique_ptr<caspian::Backend> sim = std::make_unique<caspian::UsbCaspian>(false);

    // Read network from JSON file
    auto net = std::make_unique<caspian::Network>(w*h);
    auto netptr = net.get();
    std::ifstream fin;
    fin.open(network_filename.c_str());
    if (fin.fail()) {
        std::cerr << "Couldn't open network file: " << network_filename;
        exit(1);
    }
    nlohmann::json j;
    fin >> j;
    netptr->from_json(j);

    // Configure the simulator with the new network
    auto cfg_start = std::chrono::system_clock::now();
    sim->configure(netptr);

    for (size_t i = 0; i < net->num_outputs(); i++) {
        sim->track_timing(i);
    }

    auto cfg_end = std::chrono::system_clock::now();

    int cycles = 3*w + 2*h;
    printf("Width: %d Height: %d Cycles: %d\n", w, h, cycles);
    printf("Neurons: %zu Synapses: %zu\n", net->num_neurons(), net->num_synapses());
    printf("Configuration Time: %lf us\n", (cfg_end - cfg_start).count() / 1000.0);

    uint64_t accumulations = 0;
    uint64_t fires = 0;
    uint64_t outputs = 0;
    uint64_t active_cycles = 0;

    std::vector<std::chrono::duration<double>> sim_times;

    for (int r = 0; r < runs; ++r) {

        auto sim_start = std::chrono::system_clock::now();

        // Queue up inputs
        for (int i = 0; i < h; ++i) {
            sim->apply_input(i, 255, i);
        }

        // Simulate with sufficient time
        sim->simulate(cycles);
        auto sim_end = std::chrono::system_clock::now();

        std::chrono::duration<double> sim_time = sim_end - sim_start;
        printf("Simulate %4d: %lf s\n", r, sim_time.count());
        //fmt::print("Simulate {:4d}: {} s\n", r, sim_time.count());
        sim_times.push_back(sim_time);

        accumulations += sim->get_metric("accumulate_count");
        fires += sim->get_metric("fire_count");
        active_cycles += sim->get_metric("active_clock_cycles");

        for (int i = 0; i < h; ++i) {
            printf("Output %d (%d):", i, sim->get_output_count(i));
            //fmt::print("Output {} ({}):", i, sim->get_output_count(i));
            auto outs = sim->get_output_values(i);
            for (auto o : outs) {
                printf(" %u", o);
            }
            printf("\n");
            outputs += sim->get_output_count(i);
        }

        sim->clear_activity();
    }

    std::sort(sim_times.begin(), sim_times.end());

    double ttime = 0;
    for (auto const &t : sim_times) {
        ttime += t.count();
    }
    double avg = ttime / sim_times.size();

    double avg_accum = static_cast<double>(accumulations) / static_cast<double>(runs);

    printf("\n");
    printf("---[Metrics]------------------------\n");
    printf("Average Simulate (s)     : %9.7f\n", avg);
    printf("Median Simulate  (s)     : %9.7f\n", sim_times[sim_times.size()/2].count());
    printf("Spikes                   : %lu\n", fires);
    printf("Spikes/second            : %lf\n", static_cast<double>(fires) / ttime);
    printf("Output Spikes            : %lu\n", outputs);
    printf("Accumulations            : %lu\n", accumulations);
    printf("Accumulations/second     : %.1f\n", static_cast<double>(accumulations) / ttime);

    if (active_cycles != 0) {
        // This is dependent on the actual clock speed of the dev board.
        const double clk_speed = 25000000;
        //const double clk_speed = 150000000; // previously 25000000
        double adj_time = (static_cast<double>(active_cycles) / clk_speed) / static_cast<double>(runs);
        printf("---[FPGA Metrics]-------------------\n");
        printf("Active Clock Cycles      : %lu\n", active_cycles);
        printf("Adj Runtime (s)          : %9.7f\n", adj_time);
        printf("Adj Accumulations/second : %.1f\n", avg_accum / adj_time);
    }

    return 0;
}
