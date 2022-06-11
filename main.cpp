#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <set>

#include <ftw.h>
#include <sys/resource.h>
#include <unistd.h>

#include <alpm.h>

#define ROOT_PATH "/"
#define LIBALPM_DB_PATH "/var/lib/pacman"

using namespace std;

struct package_file_t
{
	char *path;
	uid_t uid;
	gid_t gid;
	mode_t mode;

	inline package_file_t(const char *path) :
		path((char *)path)
	{}

	inline package_file_t(char *path, uid_t uid, gid_t gid, mode_t mode) :
		path(path), uid(uid), gid(gid), mode(mode)
	{}

	inline ~package_file_t()
	{
		free(path);
	}

	inline bool operator<(const package_file_t &file) const
	{
		return strcmp(this->path, file.path) < 0;
	}
};

static set<package_file_t> all_files;

static inline void reset(const package_file_t &file)
{
	chown(file.path, file.uid, file.gid);
	chmod(file.path, file.mode);
}

static inline int nftw_callback(const char *path, const struct stat *stat, int flag, struct FTW *info)
{
	package_file_t file(strdup(path));
	set<package_file_t>::iterator found_file = all_files.find(file);

	if (found_file != all_files.end())
		reset(*found_file);
	else
		printf("Warning: %s does not belong to any packages\n", file.path);

	return 0;
}

int main(int argc, const char *argv[])
{
	if (geteuid() != 0) {
		puts("Please run the program as root!");
		return 1;
	}

	alpm_errno_t err;
	alpm_handle_t *handle = alpm_initialize(ROOT_PATH, LIBALPM_DB_PATH, &err);

	if (handle == NULL) {
		printf("ERROR: Failed to initilize libalpm handle (%s)\n", alpm_strerror(err));
		return 2;
	}

	for (alpm_list_t *i = alpm_db_get_pkgcache(alpm_get_localdb(handle)); i; i = alpm_list_next(i)) {
		alpm_pkg_t *package = (alpm_pkg_t *)(i->data);

		struct archive_entry *entry;
		struct archive *archive = alpm_pkg_mtree_open(package);

		while (alpm_pkg_mtree_next(package, archive, &entry) == ARCHIVE_OK) {
			mode_t file_type = archive_entry_filetype(entry);
			if (file_type != AE_IFREG && file_type != AE_IFDIR)
				continue;

			all_files.emplace(
				strdup(archive_entry_pathname(entry) + 1),
				archive_entry_uid(entry),
				archive_entry_gid(entry),
				archive_entry_mode(entry)
			);
		}

		alpm_pkg_mtree_close(package, archive);
	}

	alpm_release(handle);

	if (argc == 1) {
		puts("Reseting all package file permissions...");
		for (const package_file_t &file : all_files)
			reset(file);
	} else if (argc == 2) {
		rlim_t max_file_fd = 4096;
		struct rlimit limit;
		if (getrlimit(RLIMIT_NOFILE, &limit) == 0)
			max_file_fd = limit.rlim_max;

		printf("Reseting file permissions under %s...\n", argv[1]);
		if (nftw(argv[1], nftw_callback, max_file_fd, FTW_PHYS) != 0) {
			printf("ERROR: Failed to traverse the directory tree! (%s)\n", strerror(errno));
			return 3;
		}
	} else {
		puts("Usage: pacreset [FILE]");
		return 4;
	}

	puts("File permissions were reset successfully!");
	return 0;
}
