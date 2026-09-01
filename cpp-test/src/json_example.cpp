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

    for (int r = 0; r < runs; ++r) {

        // Queue up inputs
        for (int i = 0; i < h; ++i) {
            proc->apply_input(i, 255, i);
        }

        // Simulate with sufficient time
        proc->simulate(cycles);

        printf("Simulate %4d\n", r);

        for (int i = 0; i < h; ++i) {
            printf("Output %d (%d):", i, proc->get_output_count(i));
            //fmt::print("Output {} ({}):", i, proc->get_output_count(i));
            auto outs = proc->get_output_values(i);
            for (auto o : outs) {
                printf(" %u", o);
            }
            printf("\n");
        }

        proc->clear_activity();
    }

    return 0;
}
