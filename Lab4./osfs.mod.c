#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xfd6ea893, "inode_init_owner" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x3f221523, "d_instantiate" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0xbf8c476a, "new_inode" },
	{ 0x49a42492, "unregister_filesystem" },
	{ 0xd5aa961b, "simple_statfs" },
	{ 0xb74a498a, "d_make_root" },
	{ 0x0586ece1, "d_splice_alias" },
	{ 0x3c587768, "current_time" },
	{ 0xfbe7861b, "memcpy" },
	{ 0x26417f80, "iput" },
	{ 0x49a42492, "register_filesystem" },
	{ 0xd272d446, "__fentry__" },
	{ 0xebb31e32, "simple_inode_init_ts" },
	{ 0x5a844b26, "__x86_indirect_thunk_rax" },
	{ 0xe8213e80, "_printk" },
	{ 0x7318d94e, "make_kuid" },
	{ 0x9479a1e8, "strnlen" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xd7a59a65, "vmalloc_noprof" },
	{ 0x7a0d1edb, "set_nlink" },
	{ 0x2435d559, "strncmp" },
	{ 0x5e9950b9, "from_kgid" },
	{ 0xc609ff70, "strncpy" },
	{ 0xe54e0a6b, "__fortify_panic" },
	{ 0xed2f3700, "default_llseek" },
	{ 0xc9dada03, "from_kuid" },
	{ 0x5a844b26, "__x86_indirect_thunk_r10" },
	{ 0x6863d638, "__insert_inode_hash" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x092a35a2, "_copy_to_user" },
	{ 0x46b141b3, "make_kgid" },
	{ 0x8fb12908, "mount_nodev" },
	{ 0xf1de9e85, "vfree" },
	{ 0x546c19d9, "validate_usercopy_range" },
	{ 0xccb33162, "generic_delete_inode" },
	{ 0xf6268c6b, "generic_file_open" },
	{ 0x1f703479, "d_parent_ino" },
	{ 0x43a349ca, "strlen" },
	{ 0x96d87a8b, "__mark_inode_dirty" },
	{ 0xed2f3700, "generic_file_llseek" },
	{ 0x5429c77f, "inode_set_ctime_to_ts" },
	{ 0x86c49e96, "nop_mnt_idmap" },
	{ 0x4749ded2, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xfd6ea893,
	0xa61fd7aa,
	0x3f221523,
	0x092a35a2,
	0xbf8c476a,
	0x49a42492,
	0xd5aa961b,
	0xb74a498a,
	0x0586ece1,
	0x3c587768,
	0xfbe7861b,
	0x26417f80,
	0x49a42492,
	0xd272d446,
	0xebb31e32,
	0x5a844b26,
	0xe8213e80,
	0x7318d94e,
	0x9479a1e8,
	0x90a48d82,
	0xd7a59a65,
	0x7a0d1edb,
	0x2435d559,
	0x5e9950b9,
	0xc609ff70,
	0xe54e0a6b,
	0xed2f3700,
	0xc9dada03,
	0x5a844b26,
	0x6863d638,
	0xd272d446,
	0x092a35a2,
	0x46b141b3,
	0x8fb12908,
	0xf1de9e85,
	0x546c19d9,
	0xccb33162,
	0xf6268c6b,
	0x1f703479,
	0x43a349ca,
	0x96d87a8b,
	0xed2f3700,
	0x5429c77f,
	0x86c49e96,
	0x4749ded2,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"inode_init_owner\0"
	"__check_object_size\0"
	"d_instantiate\0"
	"_copy_from_user\0"
	"new_inode\0"
	"unregister_filesystem\0"
	"simple_statfs\0"
	"d_make_root\0"
	"d_splice_alias\0"
	"current_time\0"
	"memcpy\0"
	"iput\0"
	"register_filesystem\0"
	"__fentry__\0"
	"simple_inode_init_ts\0"
	"__x86_indirect_thunk_rax\0"
	"_printk\0"
	"make_kuid\0"
	"strnlen\0"
	"__ubsan_handle_out_of_bounds\0"
	"vmalloc_noprof\0"
	"set_nlink\0"
	"strncmp\0"
	"from_kgid\0"
	"strncpy\0"
	"__fortify_panic\0"
	"default_llseek\0"
	"from_kuid\0"
	"__x86_indirect_thunk_r10\0"
	"__insert_inode_hash\0"
	"__x86_return_thunk\0"
	"_copy_to_user\0"
	"make_kgid\0"
	"mount_nodev\0"
	"vfree\0"
	"validate_usercopy_range\0"
	"generic_delete_inode\0"
	"generic_file_open\0"
	"d_parent_ino\0"
	"strlen\0"
	"__mark_inode_dirty\0"
	"generic_file_llseek\0"
	"inode_set_ctime_to_ts\0"
	"nop_mnt_idmap\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "A70F7F2E139E0E94BE544C6");
