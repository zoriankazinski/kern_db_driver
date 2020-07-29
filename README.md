# Kernel Device Driver For Database Access

## Specification
	open    - open a database connection and generate a cursor
	release - close the database connection after closing the cursor
	read    - run a predetermined query on the database (return results)
	write   - update query checkpoint

