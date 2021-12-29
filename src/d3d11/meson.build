d3d11_res = wrc_generator.process('version.rc')

dxgi_common_src = [
  '../dxgi/dxgi_format.cpp',
  '../dxgi/dxgi_monitor.cpp',
  '../dxgi/dxgi_swapchain.cpp',
]

d3d11_src = [
  'd3d11_annotation.cpp',
  'd3d11_blend.cpp',
  'd3d11_buffer.cpp',
  'd3d11_class_linkage.cpp',
  'd3d11_cmdlist.cpp',
  'd3d11_context.cpp',
  'd3d11_context_def.cpp',
  'd3d11_context_imm.cpp',
  'd3d11_depth_stencil.cpp',
  'd3d11_device.cpp',
  'd3d11_input_layout.cpp',
  'd3d11_main.cpp',
  'd3d11_query.cpp',
  'd3d11_rasterizer.cpp',
  'd3d11_resource.cpp',
  'd3d11_sampler.cpp',
  'd3d11_state.cpp',
  'd3d11_state_object.cpp',
  'd3d11_swapchain.cpp',
  'd3d11_texture.cpp',
  'd3d11_util.cpp',
  'd3d11_view_dsv.cpp',
  'd3d11_view_rtv.cpp',
  'd3d11_view_srv.cpp',
  'd3d11_view_uav.cpp',
]

d3d11_dll = shared_library('d3d11'+dll_ext, dxgi_common_src + d3d11_src, d3d11_res,
  name_prefix         : '',
  dependencies        : [ lib_dxgi, dxvk_dep ],
  include_directories : dxvk_include_path,
  install             : true,
  vs_module_defs      : 'd3d11'+def_spec_ext,
  override_options    : ['cpp_std='+dxvk_cpp_std])

d3d11_dep = declare_dependency(
  link_with           : [ d3d11_dll ],
  include_directories : [ dxvk_include_path ])
