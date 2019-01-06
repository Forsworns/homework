#include <iostream>
#include <string>
#include <sstream>
// #include <cstdlib>


class NullBuffer : public std::streambuf { public: int overflow(int c) { return c; } };
class NullStream : public std::ostream { public: NullStream() : std::ostream(&m_sb) {} private: NullBuffer m_sb; };

struct Args {
	// meta settings
	int verbosity;
	NullStream null_stream;

	std::ostream& debug(int level) {
		if (verbosity >= level) {
			return std::cout;
		} else {
			return null_stream;
		}
	}
};

class rayTracingArgs :public Args {
public:
	int nx, ny, ns, depth;
	int scene;
	std::string output;
	bool addFloor;
};

rayTracingArgs * parseArgs(int argc, char *argv[]) {
	rayTracingArgs * args = new rayTracingArgs();

	// Define the command line object
	TCLAP::CmdLine cmd("A simple voxelization and unvoxelization utility.", ' ', "0.0");

	TCLAP::UnlabeledValueArg<std::string> output("output", "path to save the results", false, "test", "string");
	TCLAP::ValueArg<int> scene("s", "scenes", "select a scene to test", false, 1, "int");
	TCLAP::ValueArg<int> nx("x", "nx", "resolution of x", false, 100, "int");
	TCLAP::ValueArg<int> ny("y", "ny", "resolution of y", false, 100, "int");
	TCLAP::ValueArg<int> ns("n", "ns", "sample number of a pixel", false, 3, "int");
	TCLAP::ValueArg<int> vfov("f", "fov", "field of view", false, 40, "int");
	TCLAP::ValueArg<int> depth("d", "depth", "the maximum depth for recursion", false, 50, "int");
	TCLAP::SwitchArg addFloor("a", "addFloor", "choose to add floor or not", false);
	TCLAP::MultiSwitchArg verbosity("v", "verbose", "Verbosity level. Multiple flags for more verbosity.");

	// Add args to command line object and parse
	cmd.add(nx); cmd.add(ny); cmd.add(ns); cmd.add(scene); cmd.add(verbosity); cmd.add(output);  // order matters for positional args
	cmd.parse(argc, argv);

	// store in wrapper struct
	args->output = output.getValue();
	args->scene = scene.getValue();
	args->verbosity = verbosity.getValue();
	args->nx = nx.getValue();
	args->ny = ny.getValue();
	args->ns = ns.getValue();

	return args;
}