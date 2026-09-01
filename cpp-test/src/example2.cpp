#include <network.hpp>
#include <ucaspian.hpp>
#include <utils/json_helpers.hpp>

int main()
{
    const std::string kNetworkFilename = "../networks/example.txt";

    caspian::Network net;

    // Read network from JSON file
    std::ifstream fin;
    fin.open(kNetworkFilename.c_str());
    if (fin.fail()) {
        std::cerr << "Couldn't open network file: " << kNetworkFilename;
        exit(1);
    }
    nlohmann::json j;
    fin >> j;
    net.from_json(j);

    // Configure the processor with the new network
    caspian::UsbCaspian proc;
    proc.configure(&net);

    /*
    for (size_t i = 0; i < net.num_outputs(); i++) {
        proc.track_timing(i);
    }

    const auto cycles = 3*kWidth + 2*kHeight;

    for (int r=0; r<kRuns; ++r) {

        // Queue up inputs
        for (int i = 0; i < kHeight; ++i) {
            proc.apply_input(i, 255, i);
        }

        // Simulate with sufficient time
        proc.simulate(cycles);

        printf("Simulate %4d\n", r);

        for (int i=0; i<kHeight; ++i) {
            printf("Output %d (%d):", i, proc.get_output_count(i));
            //fmt::print("Output {} ({}):", i, proc.get_output_count(i));
            auto outs = proc.get_output_values(i);
            for (auto o : outs) {
                printf(" %u", o);
            }
            printf("\n");
        }

        proc.clear_activity();
    }*/

    return 0;
}
