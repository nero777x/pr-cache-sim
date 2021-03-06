#include "application.hpp"

application::application() {}

application::application(
  const double icap_speed,
  const unsigned icap_bus_width,
  const double prc_speed,
  const taskSchedulingType task_scheduling_alg,
  const rrSelectionType rr_scheduling_alg,
  const double static_region_speed,
  const srMap_t& static_modules,
  const rrSpecMap_t& rr_spec_map,
  const double fastest_module_speed
) :
  icap_speed(icap_speed), icap_bus_width(icap_bus_width), prc_speed(prc_speed),
  task_scheduling_alg(task_scheduling_alg), rr_scheduling_alg(rr_scheduling_alg),
  static_region_speed(static_region_speed), static_modules(static_modules),
  rr_spec_map(rr_spec_map),
  simulator_speed(fastest_module_speed) { }

/* Helper Functions */

void printIndentation(unsigned indents) {
  for (unsigned i = 0; i < indents; i++) {
    std::cout << "\t";
  }
}

/* Methods */

bitstreamSizeMap_t application::getRrBitstreamSizes() {
  bitstreamSizeMap_t rr_bitstream_sizes;
  rr_bitstream_sizes.reserve(rr_spec_map.size());

  // Gather the RR's bitstream sizes from the first module of each RR.
  for (const auto rr : rr_spec_map) {
    const auto rr_id = rr.first;
    const auto& module_map = rr.second;

    // Assert that the current RR has at least one entry.
    if (module_map.size() == 0) {
      throw std::runtime_error("Target RR has no modules to draw data from.");
    }

    // Get the RR's bitstream size from one of its modules.
    const auto bitstream_size = module_map.begin()->second.bitstream_width;
    rr_bitstream_sizes[rr_id] = bitstream_size;
  }
  return rr_bitstream_sizes;
}

/**
 * Get a 2D lookup map for which tasks can be contained within which RRs and for which bitsreams.
 * @return a map[task_type_id][rr_id] with module ID lists as elements.
 */
taskRrLookupMap_t application::getRrTaskCapabilites() {
  taskRrLookupMap_t bs_capabilities;

  for (const auto rr : rr_spec_map) {
    const auto rr_id = rr.first;
    const auto& module_map = rr.second;

    // Assert that the current RR has at least one entry.
    if (module_map.size() == 0) {
      throw std::runtime_error("Target RR has no modules to draw data from.");
    }

    for (const auto rr_module : module_map) {
      const auto module_id = rr_module.first;
      const auto& tasks = rr_module.second.tasks;

      // Assert that the current module supports at least one task.
      if (tasks.size() == 0) {
        throw std::runtime_error("Target module doesn't support any tasks.");
      }

      for (const auto task : tasks) {
        const auto type_id = task.type_id;
        bs_capabilities[type_id][rr_id].push_back(module_id);
      }
    }
  }
  return bs_capabilities;
}

void application::printDetails(unsigned indents) {

  printIndentation(indents);
  std::cout << "There are " << static_modules.size() << " static regions, operating at "
        << static_region_speed << " MHz\n";
  printIndentation(indents);

  std::cout << "with each having a resident module count of: { ";
  for (auto static_it = static_modules.begin(); static_it != static_modules.end();) {
    std::cout << static_it->second;

    if (++static_it != static_modules.end())
      std::cout << ", ";
  }
  std::cout << " }\n\n";

  printIndentation(indents);
  std::cout << "There are " << rr_spec_map.size() << " reconfigurable regions\n";
  printIndentation(indents);
  std::cout << "with each region having a bitstream size of: { ";

  for (auto it = rr_spec_map.begin(); it != rr_spec_map.end();) {
    const auto rr_bitstream_size = it->second.begin()->second.bitstream_width;
    std::cout << rr_bitstream_size << " kB";

    // Add a comma at the end of the line if not the last element.
    if (++it != rr_spec_map.end()) {
      std::cout << ", ";
    }
  }
  std::cout << " }\n";

  for (const auto rr_entry : rr_spec_map) {
    printIndentation(indents);

    std::cout << "RR" << rr_entry.first << "(f) = { ";
    const auto module_map = rr_entry.second;
    for (auto it = module_map.begin(); it != module_map.end();) {

      // TODO: Fix representation.
      // std::cout << it->second.speed << " MHz";

      // Add a comma at the end of the line if not the last element.
      if (++it != module_map.end()) {
        std::cout << ", ";
      }
    }

    const auto bitstream_size = rr_entry.second.begin()->second.bitstream_width;
    std::cout << " } of size " << bitstream_size << " kB\n";
  }
}
