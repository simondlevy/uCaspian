#include <network.hpp>
#include <ucaspian.hpp>
#include <utils/json_helpers.hpp>

int main()
{
    const std::string kNetworkFilename = "networks/passnet.json";

    const int w = 5;
    const int h = 5;
    const int runs = 10;

    std::unique_ptr<caspian::Backend> proc = std::make_unique<caspian::UsbCaspian>(false);

    // Read network from JSON file
    auto net = std::make_unique<caspian::Network>(w*h);
    auto netptr = net.get();
    std::ifstream fin;
    fin.open(kNetworkFilename.c_str());
    if (fin.fail()) {
        std::cerr << "Couldn't open network file: " << kNetworkFilename;
        exit(1);
    }
    nlohmann::json j;
    fin >> j;
    netptr->from_json(j);

    // Configure the simulator with the new network
    auto cfg_start = std::chrono::system_clock::now();
    proc->configure(netptr);

    for (size_t i = 0; i < net->num_outputs(); i++) {
        proc->track_timing(i);
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

    for (int r = 0; r < runs; ++r) {

        auto sim_start = std::chrono::system_clock::now();

        // Queue up inputs
        for (int i = 0; i < h; ++i) {
            proc->apply_input(i, 255, i);
        }

        // Simulate with sufficient time
        proc->simulate(cycles);
        auto sim_end = std::chrono::system_clock::now();

        std::chrono::duration<double> sim_time = sim_end - sim_start;
        printf("Simulate %4d: %lf s\n", r, sim_time.count());

        accumulations += proc->get_metric("accumulate_count");
        fires += proc->get_metric("fire_count");
        active_cycles += proc->get_metric("active_clock_cycles");

        for (int i = 0; i < h; ++i) {
            printf("Output %d (%d):", i, proc->get_output_count(i));
            //fmt::print("Output {} ({}):", i, proc->get_output_count(i));
            auto outs = proc->get_output_values(i);
            for (auto o : outs) {
                printf(" %u", o);
            }
            printf("\n");
            outputs += proc->get_output_count(i);
        }

        proc->clear_activity();
    }

    return 0;
}
