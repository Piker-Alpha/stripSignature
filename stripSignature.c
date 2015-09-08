/*
 * Created: 31 October 2014
 * Name...: stripSignature.c
 * Author.: Pike R. Alpha
 * Purpose: Command line tool to strip CodeSignature's from MachO binaries (kexts).
 *
 * Updates:
 *			- Removed unused include files (Pike R. Alpha, September 2015).
 */

#include <mach-o/fat.h>
#include <mach-o/loader.h>

#include <CoreFoundation/CoreFoundation.h>

#define kPageSize		4096

#define RoundPage(x)	((((unsigned)(x)) + kPageSize - 1) & ~(kPageSize - 1))

//==============================================================================

struct load_command * find_load_command(struct mach_header_64 *machHeader, uint32_t targetCmd)
{
	struct load_command *loadCommand;
	
	// First LOAD_COMMAND begins after the mach header.
	loadCommand = (struct load_command *)((uint64_t)machHeader + sizeof(struct mach_header_64));
	
	while ((uint64_t)loadCommand < (uint64_t)machHeader + (uint64_t)machHeader->sizeofcmds + sizeof(struct mach_header_64))
	{
		if (loadCommand->cmd == targetCmd)
		{
			return (struct load_command *)loadCommand;
		}
		
		// Next load command.
		loadCommand = (struct load_command *)((uint64_t)loadCommand + (uint64_t)loadCommand->cmdsize);
	}
	
	// Return NULL on failure (not found).
	return NULL;
}


//==============================================================================

struct segment_command_64 * find_segment_64(struct mach_header_64 *machHeader, const char *segname)
{
	struct load_command *loadCommand;
	struct segment_command_64 *segment;
	
	// First LOAD_COMMAND begins straight after the mach header.
	loadCommand = (struct load_command *)((uint64_t)machHeader + sizeof(struct mach_header_64));

	while ((uint64_t)loadCommand < (uint64_t)machHeader + (uint64_t)machHeader->sizeofcmds + sizeof(struct mach_header_64))
	{
		// printf("loadCommand->cmdsize: 0x%llx\n", (uint64_t)loadCommand->cmdsize);

		if (loadCommand->cmd == LC_SEGMENT_64)
		{
			// Check load command's segment name.
			segment = (struct segment_command_64 *)loadCommand;

			// printf("segment->segname: %s\n", segment->segname);

			if (strcmp(segment->segname, segname) == 0)
			{
				return segment;
			}
		}
		
		// Next load command.
		loadCommand = (struct load_command *)((uint64_t)loadCommand + (uint64_t)loadCommand->cmdsize);
	}
	
	// Return NULL on failure (not found).
	return NULL;
}


//==============================================================================

int main(int argc, const char * argv[])
{
	FILE *fp									= NULL;

	unsigned char * fileBuffer					= NULL;

	struct fat_header * fatHeader				= NULL;
	struct mach_header_64 * machHeader			= NULL;
	struct segment_command_64 * linkedit		= NULL;
	struct linkedit_data_command *codeSignature	= NULL;

	unsigned long fileLength					= 0;
	unsigned long strippedFileLength			= 0;

	if (argc == 3)
	{
		// Open the source file.
		fp = fopen(argv[1], "rb");
		printf("Opening: %s\n", argv[1]);
		
		// Check file pointer.
		if (fp)
		{
			fseek(fp, 0, SEEK_END);
			fileLength = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			fileBuffer = malloc(fileLength);

			// Check filebuffer.
			if (fileBuffer)
			{
				fread(fileBuffer, fileLength, 1, fp);
				fatHeader = (struct fat_header *) fileBuffer;

				if (fatHeader->magic == FAT_CIGAM)
				{
					printf("ERROR: fat header magic mismatch\n");
					fclose(fp);
					free(fileBuffer);
					exit(-1);
				}

				machHeader = (struct mach_header_64 *)((unsigned char *)fileBuffer);
				codeSignature = (struct linkedit_data_command *)find_load_command(machHeader, LC_CODE_SIGNATURE);
				linkedit = find_segment_64(machHeader, SEG_LINKEDIT);

				if (codeSignature && linkedit)
				{
					printf("CodeSignature->cmd.....: 0x%x\n", codeSignature->cmd);
					printf("CodeSignature->cmdsize.: 0x%x\n", codeSignature->cmdsize);
					printf("CodeSignature->dataoff.: 0x%x\n", codeSignature->dataoff);
					printf("CodeSignature->datasize: 0x%x\n", codeSignature->datasize);
					
					machHeader->ncmds--;
					machHeader->sizeofcmds -= codeSignature->cmdsize;

					strippedFileLength = RoundPage(codeSignature->dataoff);

					memset((void *)fileBuffer + codeSignature->dataoff, 0, codeSignature->datasize);
					memset((void *)codeSignature, 0, codeSignature->cmdsize);

					printf("Stripped off (%ld/0x%lx) bytes\n", (fileLength - strippedFileLength), (fileLength - strippedFileLength));

					linkedit->vmsize = strippedFileLength;
					linkedit->filesize = linkedit->filesize - (fileLength - strippedFileLength);

					fp = fopen (argv[2], "wb");
					fwrite(fileBuffer, 1, strippedFileLength, fp);
					long writtenBytes = ftell(fp);
					printf("%ld/0x%lx bytes written to: %s\n\n", writtenBytes, writtenBytes, argv[2]);
					fclose(fp);

					free(fileBuffer);
				}
				else
				{
					printf("ERROR: No CodeSignature found!\n");
				}
			}
			else
			{
				printf("ERROR: Failed to allocate file buffer... exiting\nAborted!\n\n");
				fclose(fp);
			}
		}
		else
		{
			printf("ERROR: Opening of %s failed... exiting\nDone.\n", argv[1]);
		}
	}
	else
	{
		printf("Usage: %s <infile> <outfile>\n", argv[0]);
	}

	exit(-1);
}
